#include "barcodeanalyzer.h"
#include "opencv2/imgcodecs.hpp"

  #define DebugShowImages 0

#define ZXING_EXPERIMENTAL_API 1
#define EXPECTED_REEL_RADIUS (900)
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> distr(0, 255);

static cv::Scalar getcolor()
{
  return cv::Scalar(distr(gen), distr(gen), distr(gen));
}
static cv::Scalar whitecolor(255, 255, 255);

bool BarcodeAnalyzer::saveTo(QString jsonstr,const QString &filePath) const {
    
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    file.write(jsonstr.toUtf8());
    file.close();
    return true;
}



void BarcodeAnalyzer::setImgPath(const QString &path)
{
  if (m_imgPath != path)
  {
    m_imgPath = path;
    emit imagePathChanged();
  }
}

QString BarcodeAnalyzer::processImage(const QString &fileUrlOrPath)
{

  //0 ЧТЕНИЕ ИЗОБРАЖЕНИЯ И ПОДГОТОВКА ВЫХОДНЫХ ПУТЕЙ 
  int variant = 0;
  QString localPath = fileUrlOrPath;
  if (localPath.startsWith("file:///"))
    localPath = QUrl(localPath).toLocalFile();
  cv::Mat img = cv::imread(localPath.toStdString());
  if (img.empty())
    return QString("Ошибка: не удалось открыть %1").arg(localPath);
  QFileInfo fileInfo(localPath);
  QDateTime dt = QDateTime::currentDateTime();
  QString dateTimeString = dt.toString("yy-MM-dd_HH-mm-ss");
  QString resultReport = QString("");
  if (dateTimeString.isEmpty() || dateTimeString.isNull())
    dateTimeString = QString("yy-MM-dd_HH-mm-ss");
  QString outputImgName = fileInfo.absolutePath() + QString("/") +
                          dateTimeString + QString(".") + fileInfo.suffix();
  QString outputJsonName = fileInfo.absolutePath() + QString("/") +
                           dateTimeString + QString(".json");
  cout << "outputImgName=" << outputImgName.toStdString() << endl;
  cout << "outputJsonName=" << outputJsonName.toStdString() << endl;
  // 1 ОБРЕЗКА ПО КАТУШКЕ
  cv::Mat image = cropImageByReel(img);
  // 2 ВЫЧИСЛЕНИЕ УГЛА
  double angle = alignImageAngle(image);
  // 3 ПОВОРОТ ПО УГЛУ ДО КРАТНОГО 90
  cv::Point2f center((float)image.cols / 2.0f, (float)image.rows / 2.0f);
  cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);
  cv::Mat rotated;
  cv::warpAffine(image, rotated, M, image.size(), cv::INTER_LINEAR,
                 cv::BORDER_REPLICATE);
  image = rotated.clone();
  shortShow(image);

  // 4 УСТАНОВКА ЭКСПОЗИЦИИ И УРОВНЯ ЧЕРНОГО В НЕСКОЛЬКИХ ВАРИАНТАХ

  struct exposureParams
  {
    int exposure;
    int blackLevel;
  };
  std::vector<exposureParams> exposureVariants = {
      {7, 55}, {5, 55}, {5, 40}, {4, 40}, {1, 0}
    };
  // 5 ПРИМЕНЕНИЕ ВАРИАНТОВ ЭКСПОЗИЦИИ К НЕСКОЛЬКИМ КАРТИНКАМ В ВЕКТОРЕ

  std::vector<cv::Mat> exposuredImgs;
  exposuredImgs.reserve(exposureVariants.size());
  for (const auto &params : exposureVariants)
  {
    cv::Mat dst;
    adjustExposureWithBlackLevel(image, dst, params.exposure, params.blackLevel);
    exposuredImgs.push_back(dst);
    shortShow(dst);
  }
  // cv::Mat exposured;
  
  // adjustExposureWithBlackLevel(image, exposured, exposureVariants[2].exposure, exposureVariants[2].blackLevel);

// exposuredImgs


  // 6 УСТАНОВКА ОБЩИХ ПАРАМЕТРОВ ДЛЯ ПРЕОБРАЗОВАНИЯ ИЗОБРАЖЕНИЯ

  uint8_t maxBarcodesQty = 0;
  int maxQtyVariant = 0;
  cv::Mat BestExposuredImg;
  ZXing::Barcodes bestBarcodes;
  ZXing::ReaderOptions options;
  options.setTryRotate(true);
  options.setBinarizer(ZXing::Binarizer::LocalAverage);
  options.setFormats(ZXing::BarcodeFormat::AllLinear);

  // 7 ЗАПИСЬ МАКСИМАЛЬНОГО КОЛ-ВА ШТРИХКОДОВ, ЗАПИСЬ ЛУЧШЕГО СПИСКА БАРКОДОВ СРЕДИ ВСЕХ ВАРИАНТОВ ЭКСПОЗИЦИИ
  int barcodeQtys[exposuredImgs.size()] = {0};

  for (size_t i = 0; i < exposuredImgs.size(); ++i)
  {
    printf("exposuredImgs.size() = %d\n", exposuredImgs.size());
    auto imgv = ZXing::ImageView(exposuredImgs[i].data, exposuredImgs[i].cols, exposuredImgs[i].rows, ZXing::ImageFormat::Lum);
    auto brcds = ZXing::ReadBarcodes(imgv, options);
    
    barcodeQtys[i] = brcds.size();
    printf("barcodeQtys[%d].size() = %d\n", i, brcds.size());

    if (brcds.size() > maxBarcodesQty)
    {
      maxQtyVariant = i;
      maxBarcodesQty = brcds.size();
      BestExposuredImg = exposuredImgs[i].clone();
      auto BestImgv = ZXing::ImageView(BestExposuredImg.data, BestExposuredImg.cols,
                                       BestExposuredImg.rows, ZXing::ImageFormat::Lum);
      bestBarcodes = ZXing::ReadBarcodes(BestImgv, options);
          shortShow(BestExposuredImg);

    }
  }
      // BestExposuredImg = exposured.clone();
      // auto BestImgv = ZXing::ImageView(BestExposuredImg.data, BestExposuredImg.cols,
      //                                  BestExposuredImg.rows, ZXing::ImageFormat::Lum);
      // bestBarcodes = ZXing::ReadBarcodes(BestImgv, options);



    if (bestBarcodes.empty())
    return QString("Штрихкоды не найдены.");

  // 8 СОРТИРОВКА БАРКОДОВ ПО ШИРИНЕ И ПОДГОТОВКА ИНФОРМАЦИИ ДЛЯ ВЫВОДА
  BarcodeList bclist;
  resultReport.append(QString("Найдено штрихкодов: %1\n").arg(bestBarcodes.size()));
  for (size_t idx = 0; idx < bestBarcodes.size(); ++idx)
  {
    const auto &barcode = bestBarcodes[idx];
    if (!barcode.isValid())
      continue;
    bclist.items.emplace_back();
    bclist.items.back().set(barcode);
    bclist.items.back().setnum(idx);
  }
  std::sort(bclist.items.begin(), bclist.items.end(),
            [](const MyBarcodeInfo &a, const MyBarcodeInfo &b)
            {
              return a.width > b.width;
            });
  // 9 ВЫВОД ИНФОРМАЦИИ НА ИЗОБРАЖЕНИЕ И ПОДГОТОВКА ОТЧЕТА
  cvtColor(BestExposuredImg, BestExposuredImg, cv::COLOR_GRAY2BGR);

  for (size_t idx = 0; idx < bclist.items.size(); ++idx)
  {
    const auto &barcode = bestBarcodes[bclist.items[idx].localNum];
    resultReport += QString("[%1]:").arg(idx + 1);
    resultReport += bclist.items[idx].ToQstring();

    auto position = barcode.position();
    std::vector<cv::Point> points;
    cv::Scalar currentcolor = getcolor();
    for (int i = 0; i < 4; ++i)
    {
      points.push_back(cv::Point(position[i].x, position[i].y));
    }
    for (size_t i = 0; i < 4; ++i)
    {
      cv::line(BestExposuredImg, points[i], points[(i + 1) % 4], currentcolor, 5);
    }
    std::string label = "#" + std::to_string(idx + 1);
    cv::putText(BestExposuredImg, label, points[0] + cv::Point(0, -12),
                cv::FONT_HERSHEY_SIMPLEX, 2.0, currentcolor, 4);
  }

  // 10 СОХРАНЕНИЕ ИЗОБРАЖЕНИЯ И ОТЧЕТА, СРАВНЕНИЕ С ШАБЛОНАМИ ПРОИЗВОДИТЕЛЕЙ

  QString FoundTemplate = findMatchWithinTemplates(bclist);
  resultReport += QString("This pattern matches template: ") + FoundTemplate;
  cout << "This pattern matches template: " << FoundTemplate.toStdString()
       << "\n";

  // bclist.saveToFile(outputJsonName);
  cv::imwrite(outputImgName.toStdString(), BestExposuredImg);
  cout << "getImagePath().toStdString()=" << getImgPath().toStdString() << endl;
  QString finalImgPath = QString("file:///") + fileInfo.absolutePath() + QString("/") +
                         dateTimeString + QString(".") + fileInfo.suffix();
  setImgPath(finalImgPath);
  return resultReport;
}











QString
BarcodeAnalyzer::loadJsonStringFromTemplateFile(const QString &filePath)
{
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly))
    return nullptr;
  const QByteArray data = file.readAll();
  file.close();
  QJsonParseError err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &err);
  QString jsonText = doc.toJson();
  return jsonText;
}
void BarcodeAnalyzer::adjustExposureWithBlackLevel(const cv::Mat &src,
                                                   cv::Mat &dst, float exposure,
                                                   int blackLevel)
{
  // Создаем таблицу поиска (LUT) на 256 значений
  cv::Mat lookUpTable(1, 256, CV_8U);
  uchar *p = lookUpTable.ptr();

  for (int i = 0; i < 256; ++i)
  {
    // 1. Сдвигаем уровень черного
    int correctedValue = i - blackLevel;

    // 2. Применяем коэффициент экспозиции
    float exposedValue = static_cast<float>(correctedValue) * exposure;

    // 3. Ограничиваем диапазон строго от 0 до 255 (saturate_cast)
    p[i] = cv::saturate_cast<uchar>(exposedValue);
  }

  // Применяем таблицу ко всему изображению (работает мгновенно)
  cv::LUT(src, lookUpTable, dst);
}

cv::Mat BarcodeAnalyzer::cropImageByReel(const cv::Mat &src)
{

  double coeff = 0.2;

  int expectedRadius = EXPECTED_REEL_RADIUS * coeff;
  int expectedRadiusMin = expectedRadius * 0.75;
  int expectedRadiusMax = expectedRadius * 1.25;
  int minArea = expectedRadiusMin * expectedRadiusMin * 3.1415;
  int maxArea = expectedRadiusMax * expectedRadiusMax * 3.1415;
  int needArea = expectedRadius * expectedRadius * 3.1415;

  using namespace cv;
  Mat mat = src.clone();
  resize(src, mat, Size(0, 0), coeff, coeff, INTER_LINEAR);
  cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
  shortShow(mat);
  std::vector<cv::Vec3f> circles;
  HoughCircles(mat, circles, cv::HOUGH_GRADIENT, 1, mat.rows / 8, 100, 30,
               expectedRadiusMin, expectedRadiusMax);
  if (circles.size() == 0)
    return src;
  for (int i = 0; i < circles.size(); i++)
  {
    cv::circle(mat, {(int)circles[i][0], (int)circles[i][1]},
               (int)circles[i][2], (distr(gen), distr(gen), distr(gen)), 1);
  }
  int cx = cvRound(circles[0][0]);
  int cy = cvRound(circles[0][1]);
  int r = cvRound(circles[0][2]);

  cv::Rect roiRectC(cx - r, cy - r, 2 * r, 2 * r);
  cv::Rect bounds(0, 0, mat.cols, mat.rows);
  roiRectC &= bounds;
  mat = mat(roiRectC).clone();

  cv::Rect trueRoi;
  trueRoi.x = roiRectC.x / coeff;
  trueRoi.y = roiRectC.y / coeff;
  trueRoi.width = roiRectC.width / coeff;
  trueRoi.height = roiRectC.height / coeff;

  cv::Mat croppedSrc = src(trueRoi).clone();
  cvtColor(croppedSrc, croppedSrc, cv::COLOR_BGR2GRAY);

  return croppedSrc;
}
double BarcodeAnalyzer::alignImageAngle(const cv::Mat &src)
{
  std::vector<cv::Point> corners;
  cv::Mat srcImgToDrawOn;
  cv::Mat srccpy = src.clone();
  cv::cvtColor(src, srcImgToDrawOn, cv::COLOR_GRAY2BGR);
  double minValue, maxValue;

  cv::Mat gradX, gradY, gradient;
  cv::Sobel(srccpy, gradX, CV_8UC1, 1, 0, -1);
  cv::Sobel(srccpy, gradY, CV_8UC1, 0, 1, -1);
  cv::subtract(gradX, gradY, gradient);
  cv::minMaxLoc(gradient, &minValue, &maxValue);
  shortShow(gradient);


  cv::threshold(gradient, gradient, maxValue * 0.95, 255, cv::THRESH_BINARY);
  shortShow(gradient);


  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
  cv::morphologyEx(gradient, gradient, cv::MORPH_CLOSE, kernel);
  shortShow(gradient);

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(gradient, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
  double bestArea;
  cv::RotatedRect bestRect;
  bool found;
  std::vector<cv::Point> bestcontour;

  for (int i=0; i< contours.size() ; i++)
  {
    auto contour= contours.at(i);
    cv::RotatedRect rr = cv::minAreaRect(contour);
    cv::Size2f s = rr.size;
    double area = cv::contourArea(contour);
    float w = std::max(s.width, s.height), h = std::min(s.width, s.height);
    
    if ((area < 1000) || (area > 10000))
      continue;
    if (h == 0 || w == 0)
      continue;
    if (w / h < 2.0)
      continue;
    if (contour.size())
    continue;
    if (area > bestArea)
    {
      bestcontour= contour;
      bestArea = area;
      bestRect = rr;
      found = true;
      // cv::drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
      shortShow(srcImgToDrawOn);
 
    }
  }
  if (found)
  {
     cv::drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
      shortShow(srcImgToDrawOn);

    contours.clear();
    std::vector<cv::Point2f> rectpnts;
    cv::Point2f pts[4];
    bestRect.points(pts);
    std::vector<cv::Point> rect_i;
    for (int i = 0; i < 4; i++)
    {
      rect_i.emplace_back(cvRound(pts[i].x), cvRound(pts[i].y));
    }

    contours = {rect_i};
  }
  return bestRect.angle;
}

void BarcodeAnalyzer::shortShow(const cv::Mat &matrix)
{
#ifdef DebugShowImages
  cv::namedWindow("shortShow", cv::WINDOW_KEEPRATIO | cv::WINDOW_NORMAL);
  cv::moveWindow("shortShow", 250, 0);
  cv::imshow("shortShow", matrix);
  cv::waitKey(1000);
  cv::destroyWindow("shortShow");
#endif
}

cv::Mat BarcodeAnalyzer::detectAndDeskewBarcode(const cv::Mat &src,
                                                int variant)
{

  // cv::Mat gray = src.clone();
  // shortShow(gray);
  // cv::Mat gradX, gradY, gradient;
  // cv::Sobel(gray, gradX, CV_32F, 1, 0, -1);
  // cv::Sobel(gray, gradY, CV_32F, 0, 1, -1);
  // cv::subtract(gradX, gradY, gradient);
  // // cv::convertScaleAbs(gradient, gradient);
  // shortShow(gradient);

  // cv::Mat blurred, thresh;

  // cv::blur(gradient, blurred, cv::Size(9, 9));
  // shortShow(blurred);

  // switch (variant)
  // {
  // case deskewVariant::threshold210:
  //   cv::threshold(blurred, thresh, 190, 255, cv::THRESH_BINARY);
  //   break;
  // case deskewVariant::threshold220:
  //   cv::threshold(blurred, thresh, 210, 255, cv::THRESH_BINARY);
  //   break;
  // case deskewVariant::threshold230:
  //   cv::threshold(blurred, thresh, 230, 255, cv::THRESH_BINARY);
  //   break;
  // case deskewVariant::threshold210_e:
  //   cv::threshold(blurred, thresh, 190, 255, cv::THRESH_BINARY);
  //   break;
  // case deskewVariant::threshold220_e:
  //   cv::threshold(blurred, thresh, 210, 255, cv::THRESH_BINARY);
  //   break;
  // case deskewVariant::threshold230_e:
  //   cv::threshold(blurred, thresh, 230, 255, cv::THRESH_BINARY);
  //   break;
  // case deskewVariant::thresholdOtsu:
  //   cv::threshold(blurred, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
  //   break;
  // case deskewVariant::adaptive:
  //   cv::adaptiveThreshold(blurred, thresh, 255, cv::ADAPTIVE_THRESH_MEAN_C,
  //                         cv::THRESH_BINARY_INV, 11, 2);
  //   break;
  // default:
  //   cv::threshold(blurred, thresh, 200, 255, cv::THRESH_BINARY);
  //   break;
  // }
  // shortShow(thresh);

  // cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
  // cv::Mat bw;
  // cv::morphologyEx(thresh, bw, cv::MORPH_CLOSE, kernel);
  // shortShow(bw);

  // std::vector<std::vector<cv::Point>> contours;

  // int i = -1;
  // cv::findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  // cv::Mat bwc = src.clone();
  // double bestArea = 0;
  // cv::RotatedRect bestRect;
  // bool found = false;
  // for (auto &c : contours)
  // {
  //   i++;

  //   double area = cv::contourArea(c);

  //   if (area < 1000)
  //     continue;
  //   cv::RotatedRect rr = cv::minAreaRect(c);
  //   cv::Size2f s = rr.size;
  //   float w = std::max(s.width, s.height), h = std::min(s.width, s.height);
  //   if (h == 0 || w == 0)
  //     continue;
  //   float ratio = w / h;
  //   if (ratio < 2.0f)
  //     continue;
  //   if (area > bestArea)
  //   {
  //     bestArea = area;
  //     bestRect = rr;
  //     found = true;
  //   }
  // }
  // if (found)
  // {

  //   contours.clear();
  //   std::vector<cv::Point2f> rectpnts;
  //   cv::Point2f pts[4];
  //   bestRect.points(pts);
  //   std::vector<cv::Point> rect_i;
  //   for (int i = 0; i < 4; i++)
  //   {
  //     rect_i.emplace_back(cvRound(pts[i].x), cvRound(pts[i].y));
  //   }
  //   // rect_i.reserve(rectpnts.size());
  //   // for (auto &p : rectpnts)
  //   //   rect_i.emplace_back(cv::Point(cvRound(p.x), cvRound(p.y)));
  //   contours = {rect_i};
  //   cv::drawContours(bwc, contours, 0, whitecolor, 5);
  //   shortShow(bwc);
  // }
  // shortShow(bwc);

  // float angle = bestRect.angle;
  // if (bestArea == 0)
  //   angle = 0;

  // cv::Size2f rectSize = bestRect.size;
  // if (rectSize.width < rectSize.height)
  // {
  //   angle += 90.0f;
  // }

  // cv::Point2f center((float)src.cols / 2.0f, (float)src.rows / 2.0f);
  // cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);
  // shortShow(bwc);

  // cv::Mat rotated;
  // cv::warpAffine(src, rotated, M, src.size(), cv::INTER_LINEAR,
  //                cv::BORDER_REPLICATE);
  // shortShow(rotated);

  cv::Mat exposured;
  adjustExposureWithBlackLevel(src, exposured, 7, 55);

  shortShow(exposured);
  // cv::namedWindow("exposured", cv::WINDOW_KEEPRATIO | cv::WINDOW_NORMAL |
  //                                    cv::WINDOW_GUI_EXPANDED);

  return exposured;
}
QStringList BarcodeAnalyzer::loadAllTemlateFileNames() {
  QDir dir(templatesFolder);
  QStringList nameFilters;
  nameFilters.append("*.json");
  dir.setNameFilters(nameFilters);
  QStringList jsonFiles = dir.entryList();
  return jsonFiles;
}

QStringList BarcodeAnalyzer::getFolderContent(const QString &filePath)
{
  QDir dir(filePath);
  dir.setFilter(QDir::Files);
 
    
  
  // if (filter.isEmpty() )  
  //  nameFilters.append("*.");
  //  nameFilters.append(filter);
  
  // dir.setNameFilters(nameFilters);
  QStringList fileslist = dir.entryList();
  return fileslist;
}


void BarcodeAnalyzer::refreshMaterialTemplates()
{

  QDir dir(templatesFolder);
  QStringList nameFilters;
  nameFilters.append("*.json");
  dir.setNameFilters(nameFilters);
  QStringList jsonFiles = dir.entryList();
  QStringList jsonFilesFullPaths;
  for (int i = 0; i < jsonFiles.size(); i++)
  {
    jsonFilesFullPaths.append(dir.filePath(jsonFiles[i]));
  }
  if (materialTemplates == jsonFilesFullPaths)
    return;
  materialTemplates = jsonFilesFullPaths;
  materialTemplatesNames = jsonFiles;
  emit materialTemplatesChanged();
  emit materialTemplatesNamesChanged();
}

QString BarcodeAnalyzer::findMatchWithinTemplates(BarcodeList &readedBarcode)
{
  QStringList myTemplatesList = GetMaterialTemplates();
  QString matchedTemplateName;
  for (int i = 0; i < myTemplatesList.size(); i++)
  {
    BarcodeList templateBarcodeList;
    templateBarcodeList.loadFromFile(myTemplatesList[i]);
    if (readedBarcode.isfullMatch(templateBarcodeList) == true)
    {
      matchedTemplateName.append(templateBarcodeList.name);
      readedBarcode.setName(matchedTemplateName);
      break;
    }
  }
  return matchedTemplateName;
}
