#include "barcodeanalyzer.h"
#include "opencv2/imgcodecs.hpp"

//   #define DebugShowImages 0

// #define CROP_IMAGE_BY_REEL 1

#define EXPECTED_REEL_RADIUS (900)

static std::random_device rd;

static std::mt19937 gen(rd());

static std::uniform_int_distribution<int> distr(0, 255);

#include <cmath>
#include <cstdint>
#include <opencv2/opencv.hpp>

static double g_hue = 0.0;

static cv::Scalar getcolor();

static cv::Scalar whitecolor(255, 255, 255);

QString BarcodeAnalyzer::processTemplateImage(QString templateName) {

    // 0 ЧТЕНИЕ ИЗОБРАЖЕНИЯ И ПОДГОТОВКА ВЫХОДНЫХ ПУТЕЙ
    int variant = 0;
    QString localPath = templatesPath + templateName + ".bmp";
    qDebug() << "localpath:" << localPath;

    cv::Mat img = cv::imread(localPath.toStdString());
    if (img.empty()) return QString("Ошибка: не удалось открыть %1").arg(localPath);
    QFileInfo fileInfo(localPath);
    QDateTime dt = QDateTime::currentDateTime();
    QString dateTimeString = dt.toString("yy-MM-dd_HH-mm-ss");
    QString resultReport = QString("");
    if (dateTimeString.isEmpty() || dateTimeString.isNull()) dateTimeString = QString("yy-MM-dd_HH-mm-ss");
    QString outputImgName = templatesPath + templateName + "_result" + ".bmp";
    QString outputJsonName = fileInfo.absolutePath() + QString("/") + templateName + QString(".json");

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
    cv::warpAffine(image, rotated, M, image.size(), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    image = rotated.clone();
    shortShow(image);

    // 4 УСТАНОВКА ЭКСПОЗИЦИИ И УРОВНЯ ЧЕРНОГО В НЕСКОЛЬКИХ ВАРИАНТАХ

    struct exposureParams {
        int exposure;
        int blackLevel;
    };
    std::vector<exposureParams> exposureVariants = { {1, 0}};
    // 5 ПРИМЕНЕНИЕ ВАРИАНТОВ ЭКСПОЗИЦИИ К НЕСКОЛЬКИМ КАРТИНКАМ В ВЕКТОРЕ

    std::vector<cv::Mat> exposuredImgs;
    exposuredImgs.reserve(exposureVariants.size());
    for (const auto &params : exposureVariants) {
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
    options.setTryDownscale(false);
    options.setBinarizer(ZXing::Binarizer::LocalAverage);
    options.setFormats(ZXing::BarcodeFormat::AllLinear);

    // 7 ЗАПИСЬ МАКСИМАЛЬНОГО КОЛ-ВА ШТРИХКОДОВ, ЗАПИСЬ ЛУЧШЕГО СПИСКА БАРКОДОВ СРЕДИ ВСЕХ ВАРИАНТОВ ЭКСПОЗИЦИИ
    int barcodeQtys[exposuredImgs.size()] = {0};

    for (size_t i = 0; i < exposuredImgs.size(); ++i) {
        printf("exposuredImgs.size() = %d\n", exposuredImgs.size());
        auto imgv = ZXing::ImageView(exposuredImgs[i].data, exposuredImgs[i].cols, exposuredImgs[i].rows, ZXing::ImageFormat::Lum);
        auto brcds = ZXing::ReadBarcodes(imgv, options);

        barcodeQtys[i] = brcds.size();
        printf("barcodeQtys[%d].size() = %d\n", i, brcds.size());

        if (brcds.size() > maxBarcodesQty) {
            maxQtyVariant = i;
            maxBarcodesQty = brcds.size();
            BestExposuredImg = exposuredImgs[i].clone();
            auto BestImgv = ZXing::ImageView(BestExposuredImg.data, BestExposuredImg.cols, BestExposuredImg.rows, ZXing::ImageFormat::Lum);
            bestBarcodes = ZXing::ReadBarcodes(BestImgv, options);
            shortShow(BestExposuredImg);
        }
    }
    // BestExposuredImg = exposured.clone();
    // auto BestImgv = ZXing::ImageView(BestExposuredImg.data, BestExposuredImg.cols,
    //                                  BestExposuredImg.rows, ZXing::ImageFormat::Lum);
    // bestBarcodes = ZXing::ReadBarcodes(BestImgv, options);

    if (bestBarcodes.empty()) return QString("Штрихкоды не найдены.");

    // 8 СОРТИРОВКА БАРКОДОВ ПО ШИРИНЕ И ПОДГОТОВКА ИНФОРМАЦИИ ДЛЯ ВЫВОДА
    BarcodeList bclist;
    resultReport.append(QString("Найдено штрихкодов: %1\n").arg(bestBarcodes.size()));
    for (size_t idx = 0; idx < bestBarcodes.size(); ++idx) {
        const auto &barcode = bestBarcodes[idx];
        if (!barcode.isValid()) continue;
        bclist.items.emplace_back();
        bclist.items.back().set(barcode);
        bclist.items.back().setnum(idx);
    }
    std::sort(bclist.items.begin(), bclist.items.end(), [](const MyBarcodeInfo &a, const MyBarcodeInfo &b) { return a.width > b.width; });
    // 9 ВЫВОД ИНФОРМАЦИИ НА ИЗОБРАЖЕНИЕ И ПОДГОТОВКА ОТЧЕТА
    cvtColor(BestExposuredImg, BestExposuredImg, cv::COLOR_GRAY2BGR);

    for (size_t idx = 0; idx < bclist.items.size(); ++idx) {
        const auto &barcode = bestBarcodes[bclist.items[idx].localNum];
        resultReport += QString("[%1]:").arg(idx + 1);
        resultReport += bclist.items[idx].ToQstring();

        auto position = barcode.position();
        std::vector<cv::Point> points;
        cv::Scalar currentcolor = getcolor();
        for (int i = 0; i < 4; ++i) {
            points.push_back(cv::Point(position[i].x, position[i].y));
        }
        for (size_t i = 0; i < 4; ++i) {
            cv::line(BestExposuredImg, points[i], points[(i + 1) % 4], currentcolor, 5);
        }
        std::string label = "#" + std::to_string(idx + 1);
        cv::putText(BestExposuredImg, label, points[0] + cv::Point(-12, -12), cv::FONT_HERSHEY_SIMPLEX, 2.0, currentcolor, 4);
    }

    // 10 СОХРАНЕНИЕ ИЗОБРАЖЕНИЯ И ОТЧЕТА, СРАВНЕНИЕ С ШАБЛОНАМИ ПРОИЗВОДИТЕЛЕЙ

    QString FoundTemplate = findMatchWithinTemplates(bclist);
    resultReport += QString("This pattern matches template: ") + FoundTemplate;
    cout << "This pattern matches template: " << FoundTemplate.toStdString() << "\n";

    bclist.saveToFile(outputJsonName);
    cv::imwrite(outputImgName.toStdString(), BestExposuredImg);
    cout << "getImagePath().toStdString()=" << getImgPath().toStdString() << endl;

    return resultReport;
}

bool BarcodeAnalyzer::saveTo(QString jsonstr, const QString &filePath) const {

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    file.write(jsonstr.toUtf8());
    file.close();
    return true;
}

bool BarcodeAnalyzer::copyImageToTemplatesFolderAsChosenTemplateImage(QString templateName, QUrl chosenImage) {
    QString dstPath;
    if (!chosenImage.isValid()) return false;

    QString srcPath;
    qDebug() << chosenImage.toLocalFile();

    srcPath = chosenImage.toLocalFile();
    QFileInfo srcFileInfo(srcPath);

    dstPath = templatesFolder + templateName + "." + srcFileInfo.suffix();

    if (QFile::exists(dstPath)) {
        QFile::remove(dstPath);
    }
    if (QFile::copy(srcPath, dstPath) == false) return false;
    return true;
}

bool BarcodeAnalyzer::addNewEmptyTemplateJson( const QString &filePath) {
    QString fpath = filePath +  "new_template.json";
    QString jsonstr =  "{  \"name\": \"new_template\", \"barcodes\": [] }";
    saveTo(jsonstr, fpath);
    refreshMaterialTemplates();
    return true;
}

bool BarcodeAnalyzer::makeTemplateFromSrcImage(QString templateName) {}

void BarcodeAnalyzer::setImgPath(const QString &path) {
    if (m_imgPath != path) {
        m_imgPath = path;
        emit imagePathChanged();
    }
}

QString BarcodeAnalyzer::processImage(const QString &fileUrlOrPath) {
    int variant = 0;
    QString localPath = fileUrlOrPath;
    if (localPath.startsWith("file:///")) localPath = QUrl(localPath).toLocalFile();
    cv::Mat img = cv::imread(localPath.toStdString());
    if (img.empty()) return QString("Ошибка: не удалось открыть %1").arg(localPath);
    QFileInfo fileInfo(localPath);
    QDateTime dt = QDateTime::currentDateTime();
    QString dateTimeString = dt.toString("yyyy-MM-dd_HH-mm-ss");
    QString resultReport = QString("");
    if (dateTimeString.isEmpty() || dateTimeString.isNull()) dateTimeString = QString("yyyy-MM-dd_HH-mm-ss");
    QString outputImgName = fileInfo.absolutePath() + QString("/") + dateTimeString +"_result"+ QString(".") + fileInfo.suffix();
    QString outputJsonName = fileInfo.absolutePath() + QString("/") + dateTimeString + QString(".json");
    cout << "outputJsonName=" << outputJsonName.toStdString() << endl;
    // 1 ОБРЕЗКА ПО КАТУШКЕ
//    double angle1 = DetectRectangle(img);
    cv::Mat image = cropImageByReel(img);

    // 2 ВЫЧИСЛЕНИЕ УГЛА
    double angle = DetectRectangle(img);
    // 3 ПОВОРОТ ПО УГЛУ ДО КРАТНОГО 90
    cv::Point2f center((float)image.cols / 2.0f, (float)image.rows / 2.0f);
    cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);

    double absCos = std::abs(M.at<double>(0,0));
    double absSin = std::abs(M.at<double>(0,1));
    int newW = int(image.rows * absSin + image.cols * absCos);
    int newH = int(image.rows * absCos + image.cols * absSin);
    M.at<double>(0,2) += (newW - image.cols) / 2.0;
    M.at<double>(1,2) += (newH - image.rows) / 2.0;

    cv::Mat rotated;
    cv::warpAffine(image, rotated, M, cv::Size(newW, newH), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    // rotated.clone();
    shortShow(rotated);

    // 4 УСТАНОВКА ЭКСПОЗИЦИИ И УРОВНЯ ЧЕРНОГО В НЕСКОЛЬКИХ ВАРИАНТАХ

    struct exposureParams {
        int exposure;
        int blackLevel;
    };
    // std::vector<exposureParams> exposureVariants = {{7, 55}, {5, 55}, {5, 40}, {4, 40}, {1, 0}};
    std::vector<exposureParams> exposureVariants = {{1, 0}};
    // 5 ПРИМЕНЕНИЕ ВАРИАНТОВ ЭКСПОЗИЦИИ К НЕСКОЛЬКИМ КАРТИНКАМ В ВЕКТОРЕ

    std::vector<cv::Mat> exposuredImgs;
    exposuredImgs.reserve(exposureVariants.size());
    for (const auto &params : exposureVariants) {
        cv::Mat dst;
        adjustExposureWithBlackLevel(rotated, dst, params.exposure, params.blackLevel);
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
    options.setFormats(ZXing::BarcodeFormat::All);

    // 7 ЗАПИСЬ МАКСИМАЛЬНОГО КОЛ-ВА ШТРИХКОДОВ, ЗАПИСЬ ЛУЧШЕГО СПИСКА БАРКОДОВ СРЕДИ ВСЕХ ВАРИАНТОВ ЭКСПОЗИЦИИ
    int barcodeQtys[exposuredImgs.size()] = {0};

    for (size_t i = 0; i < exposuredImgs.size(); ++i) {
        printf("exposuredImgs.size() = %d\n", exposuredImgs.size());
        auto imgv = ZXing::ImageView(exposuredImgs[i].data, exposuredImgs[i].cols, exposuredImgs[i].rows, ZXing::ImageFormat::Lum);
        
 
        auto brcds = ZXing::ReadBarcodes(imgv, options);

        barcodeQtys[i] = brcds.size();
        printf("barcodeQtys[%d].size() = %d\n", i, brcds.size());

        if (brcds.size() > maxBarcodesQty) {
            maxQtyVariant = i;
            maxBarcodesQty = brcds.size();
            BestExposuredImg = exposuredImgs[i].clone();
            auto BestImgv = ZXing::ImageView(BestExposuredImg.data, BestExposuredImg.cols, BestExposuredImg.rows, ZXing::ImageFormat::Lum);
            bestBarcodes = ZXing::ReadBarcodes(BestImgv, options);
             shortShow(BestExposuredImg);
        }
    }
    if (bestBarcodes.empty()) return QString("1D Barcodes not found.");

    BarcodeList bclist;
    resultReport.append(QString("Найдено баркодов: %1\n").arg(bestBarcodes.size()));
    for (size_t idx = 0; idx < bestBarcodes.size(); ++idx) {
        const auto &barcode = bestBarcodes[idx];
        if (!barcode.isValid()) continue;
        bclist.items.emplace_back();
        bclist.items.back().set(barcode);
        bclist.items.back().setnum(idx);
    }
    std::sort(bclist.items.begin(), bclist.items.end(), [](const MyBarcodeInfo &a, const MyBarcodeInfo &b) { return a.width > b.width; });






    // 9 ВЫВОД ИНФОРМАЦИИ НА ИЗОБРАЖЕНИЕ И ПОДГОТОВКА ОТЧЕТА
    cvtColor(BestExposuredImg, BestExposuredImg, cv::COLOR_GRAY2BGR);

    for (size_t idx = 0; idx < bclist.items.size(); ++idx) {
        const auto &barcode = bestBarcodes[bclist.items[idx].localNum];
        resultReport += QString("[%1]:").arg(idx + 1);
        resultReport += bclist.items[idx].ToQstring();

        auto position = barcode.position();
        std::vector<cv::Point> points;
        cv::Scalar currentcolor = getcolor();
        for (int i = 0; i < 4; ++i) {
            points.push_back(cv::Point(position[i].x, position[i].y));
        }
        for (size_t i = 0; i < 4; ++i) {
            cv::line(BestExposuredImg, points[i], points[(i + 1) % 4], currentcolor, 5);
        }
        std::string label = "#" + std::to_string(idx + 1);
        cv::putText(BestExposuredImg, label, points[0] + cv::Point(0, -12), cv::FONT_HERSHEY_SIMPLEX, 2.0, currentcolor, 4);
    }

    // 10 СОХРАНЕНИЕ ИЗОБРАЖЕНИЯ И ОТЧЕТА, СРАВНЕНИЕ С ШАБЛОНАМИ ПРОИЗВОДИТЕЛЕЙ

    QString FoundTemplate = findMatchWithinTemplates(bclist);
    resultReport += QString("This pattern matches template: ") + FoundTemplate;
    cout << "This pattern matches template: " << FoundTemplate.toStdString() << "\n";

    // bclist.saveToFile(outputJsonName);

    cv::imwrite(outputImgName.toStdString(), BestExposuredImg);
    cout << "outputImgName.toStdString()=" << outputImgName.toStdString() << endl;
    QString finalImgPath = QString("file:///") + fileInfo.absolutePath() + QString("/") + dateTimeString + "_result" + QString(".") + fileInfo.suffix();
    setImgPath(finalImgPath);
    emit resultReady();
    return resultReport;
}

QString BarcodeAnalyzer::loadJsonStringFromTemplateFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return nullptr;
    const QByteArray data = file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    QString jsonText = doc.toJson();
    return jsonText;
}

void BarcodeAnalyzer::adjustExposureWithBlackLevel(const cv::Mat &src, cv::Mat &dst, float exposure, int blackLevel) {
    // Создаем таблицу поиска (LUT) на 256 значений
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar *p = lookUpTable.ptr();

    for (int i = 0; i < 256; ++i) {
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

cv::Mat BarcodeAnalyzer::cropImageByReel(const cv::Mat &src) {
    #ifdef CROP_IMAGE_BY_REEL 

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
    HoughCircles(mat, circles, cv::HOUGH_GRADIENT, 1, mat.rows / 8, 100, 30, expectedRadiusMin, expectedRadiusMax);
    if (circles.size() == 0) return src;
    for (int i = 0; i < circles.size(); i++) {
        cv::circle(mat, {(int)circles[i][0], (int)circles[i][1]}, (int)circles[i][2], (distr(gen), distr(gen), distr(gen)), 1);
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

    #else
     cv::Mat mat = src.clone();
    cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
        return mat;
    #endif
}
double BarcodeAnalyzer::DetectRectangle(cv::Mat src) {
using namespace std;
using namespace cv;
    
    struct exposureParams {
        int exposure;
        int blackLevel;
    };
//    exposureParams exposureVariants[]= {{1, 0},{4, 40}, {7, 55}, {5, 60}, {4, 70}, {3, 80}, };
    exposureParams exposureVariants[]= {{1, 0}};
    Mat exposedPictures[sizeof(exposureVariants)/sizeof(exposureParams)];


    for (int  i = 0; i< sizeof(exposureVariants)/sizeof(exposureParams); i++){
        adjustExposureWithBlackLevel(src, exposedPictures[i],exposureVariants[i].exposure,exposureVariants[i].blackLevel );
        shortShow(exposedPictures[i]);

    }
    


    // imshow("original Image", src);

    for (int i = 0; i < sizeof(exposureVariants)/sizeof(exposureParams); i++){
  
    Mat srcImgToDrawOn = src.clone();
    Mat srccpy = src.clone();
    //imshow("original Image", src);
    
    // Convert to graycsale
    Mat img_gray8;
    Mat img_gray;

    cvtColor(exposedPictures[i], img_gray8, COLOR_BGR2GRAY);
    shortShow(img_gray8);


    shortShow(img_gray8);

    img_gray8.convertTo(img_gray, CV_32F);                
    
    shortShow(img_gray);

    
    // Blur the image for better edge detection
    Mat img_blur;
    GaussianBlur(img_gray, img_blur, Size(5,5), 0,0);
    shortShow(img_blur);

    Mat gradX, gradY, gradient;
    Sobel(img_blur, gradX, CV_32F, 1, 0, 3);
    Sobel(img_blur, gradY, CV_32F, 0, 1, 3);
    // amplitude = sqrt(gradX^2 + gradY^2)
    Mat gradX2, gradY2, amplitude;
    multiply(gradX, gradX, gradX2);
    multiply(gradY, gradY, gradY2);
    add(gradX2, gradY2, amplitude);
    sqrt(amplitude, amplitude); // теперь amplitude — амплитуда в CV_32F
    // shortShow(amplitude);
    
    double gmin, gmax;
    minMaxLoc(amplitude, &gmin, &gmax);
    cout<< "gmin = " << gmin << "gmax = " << gmax << endl;
    amplitude.convertTo(gradient, CV_8U, 255.0/(gmax) );
    shortShow(gradient);

    
    double minValue =0; double maxValue=0;

    cv::minMaxLoc(gradient, &minValue, &maxValue);
            cout<< "minValue = " << minValue << "maxValue = " << maxValue << endl;
    threshold(gradient, gradient,0.7*maxValue, 255, THRESH_BINARY);

    //  adaptiveThreshold(gradient, gradient,255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 0);
    shortShow(gradient);
    Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(7,7));
    Mat kernelo = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));

    Mat morphology; 
    morphologyEx(gradient, morphology, cv::MORPH_CLOSE, kernel);
    shortShow(morphology);
    
    
    morphologyEx(morphology, morphology, cv::MORPH_OPEN, kernelo);
    shortShow(morphology);
    
    morphologyEx(morphology, morphology, cv::MORPH_CLOSE, kernel);
    shortShow(morphology);
    
    vector<vector<Point>> contours;
    findContours(morphology, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    double bestArea;
    double bestFill;
    RotatedRect bestRect;
    bool found;
    std::vector<Point> bestcontour;

    for (int i = 0; i < contours.size(); i++) {
        auto contour = contours.at(i);
        RotatedRect rr = minAreaRect(contour);
        Size2f s = rr.size;
        double fillPercent;
        double area = contourArea(contour);
         float w = std::max(s.width, s.height);
         float h = std::min(s.width, s.height);
                 if (h == 0 || w == 0) continue;

        if ((area < 500) || (area > 60000)) continue;
        fillPercent = area/(w*h);
        if (fillPercent < 0.7) continue;

        cout<<  "w x h = "<<w <<"x"<<h<<"="<<w*h<<endl;
        cout<< "contour area / rectangleArea = "<< area <<"/ " << w*h << "= " << fillPercent << endl; 
        if (area > bestArea) {
            bestcontour = contour;
            bestArea = area;
            bestRect = rr;
            found = true;
            // drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
            // shortShow(srcImgToDrawOn);
        }
    }
     if (found) {
        drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
         shortShow(srcImgToDrawOn);
        cout<<" bestRect.angle is "<<  bestRect.angle <<endl;

        contours.clear();
        std::vector<cv::Point2f> rectpnts;
        cv::Point2f pts[4];
        bestRect.points(pts);
        std::vector<cv::Point> rect_i;
        for (int i = 0; i < 4; i++) {
            rect_i.emplace_back(cvRound(pts[i].x), cvRound(pts[i].y));
        }
        drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
         shortShow(srcImgToDrawOn);

        contours = {rect_i};
            cout<<" bestRect.angle is "<<  bestRect.angle <<endl;
        return bestRect.angle;

    } 
    
}
    return 0;
}




double BarcodeAnalyzer::alignImageAngle(const cv::Mat &src) {
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

    for (int i = 0; i < contours.size(); i++) {
        auto contour = contours.at(i);
        cv::RotatedRect rr = cv::minAreaRect(contour);
        cv::Size2f s = rr.size;
        double area = cv::contourArea(contour);
        float w = std::max(s.width, s.height), h = std::min(s.width, s.height);

        if ((area < 1000) || (area > 10000)) continue;
        if (h == 0 || w == 0) continue;
        if (w / h < 2.0) continue;
        if (contour.size()) continue;
        if (area > bestArea) {
            bestcontour = contour;
            bestArea = area;
            bestRect = rr;
            found = true;
            // cv::drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
            shortShow(srcImgToDrawOn);
        }
    }
    if (found) {
        cv::drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
        shortShow(srcImgToDrawOn);

        contours.clear();
        std::vector<cv::Point2f> rectpnts;
        cv::Point2f pts[4];
        bestRect.points(pts);
        std::vector<cv::Point> rect_i;
        for (int i = 0; i < 4; i++) {
            rect_i.emplace_back(cvRound(pts[i].x), cvRound(pts[i].y));
        }

        contours = {rect_i};
    }
    return bestRect.angle;
}
 /*
double BarcodeAnalyzer::alignImageAngle(const cv::Mat &src) {
    std::vector<cv::Point> corners;
    
cv::Mat srcImgToDrawOn;
    cv::Mat srccpy = src.clone();
    cv::cvtColor(src, srcImgToDrawOn, cv::COLOR_GRAY2BGR);
// #else
    // cv::Mat srcImgToDrawOn = src.clone();
    // cv::Mat srccpy = src.clone();
    // cv::cvtColor(src, srccpy, cv::COLOR_BGR2GRAY);

    // #endif 
    
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

    for (const auto &c : contours) {
    double area = cv::contourArea(c);
    if (area < 500) continue;
    if (area > (src.cols * src.rows * 0.2)) continue;

    // Аппроксимация контура (упрощение вершин)
    std::vector<cv::Point> approx;
    double peri = cv::arcLength(c, true);
    cv::approxPolyDP(c, approx, 0.02 * peri, true);

    // Требуем хотя бы несколько вершин (штрих-код — не слишком простой контур)
    if (approx.size() < 4) continue;

    // Выпуклость: сравниваем площадь контура и площади выпуклой оболочки
    std::vector<cv::Point> hull;
    cv::convexHull(c, hull);
    double hullArea = cv::contourArea(hull);
    if (hullArea <= 0) continue;
    double solidity = area / hullArea; // степень заполнения выпуклой оболочки
    if (solidity < 0.4) continue;      // слишком "дырявый" — отбрасываем
    if (solidity > 1.02) continue;     // числовая стабильность (хотя обычно <=1)

    // Доп. проверка: выпуклость как булев результат
    if (!cv::isContourConvex(hull)) continue;

    cv::RotatedRect rr = cv::minAreaRect(c);
    cv::Size2f s = rr.size;
    float w = std::max(s.width, s.height);
    float h = std::min(s.width, s.height);
    if (h <= 0.0f) continue;

    double ratio = w / h;
    if (ratio < 2.0) continue;

    double rectArea = w * h;
    double fillRatio = area / rectArea;
    if (fillRatio < 0.4) continue;

    if (area > bestArea) {
        bestArea = area;
        bestRect = rr;
        found = true;
    }
}

    // for (int i = 0; i < contours.size(); i++) {
    //     auto contour = contours.at(i);
    //     cv::RotatedRect rr = cv::minAreaRect(contour);
    //     cv::Size2f s = rr.size;
    //     double area = cv::contourArea(contour);
    //     float w = std::max(s.width, s.height), h = std::min(s.width, s.height);

    //     if ((area < 1000) || (area > 10000)) continue;
    //     if (h == 0 || w == 0) continue;
    //     if (w / h < 2.0) continue;
    //     if (contour.size()) continue;
    //     if (area > bestArea) {
    //         bestcontour = contour;
    //         bestArea = area;
    //         bestRect = rr;
    //         found = true;
    //         // cv::drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
    //         shortShow(srcImgToDrawOn);
    //     }
    // }

    if (found) {
        cv::drawContours(srcImgToDrawOn, contours, -1, getcolor(), 4);
        shortShow(srcImgToDrawOn);

        contours.clear();
        std::vector<cv::Point2f> rectpnts;
        cv::Point2f pts[4];
        bestRect.points(pts);
        std::vector<cv::Point> rect_i;
        for (int i = 0; i < 4; i++) {
            rect_i.emplace_back(cvRound(pts[i].x), cvRound(pts[i].y));
        }

        contours = {rect_i};
    }
    return bestRect.angle;
}
*/


void BarcodeAnalyzer::shortShow(const cv::Mat &matrix) {
#ifdef DebugShowImages
    cv::namedWindow("shortShow", cv::WINDOW_KEEPRATIO | cv::WINDOW_NORMAL);
    cv::moveWindow("shortShow", 250, 0);
    cv::imshow("shortShow", matrix);
    cv::waitKey(10000);
    cv::destroyWindow("shortShow");
#endif
}
void BarcodeAnalyzer::shortShow(const cv::Mat &matrix, cv::String name) {
#ifdef DebugShowImages
    cv::namedWindow(name, cv::WINDOW_KEEPRATIO | cv::WINDOW_NORMAL);
    cv::moveWindow("shortShow", 250, 0);
    cv::imshow("shortShow", matrix);
    cv::waitKey(10000);
    cv::destroyWindow("shortShow");
#endif
}


QStringList BarcodeAnalyzer::loadAllTemlateFileNames() {
    QDir dir(templatesFolder);
    QStringList nameFilters;
    nameFilters.append("*.json");
    dir.setNameFilters(nameFilters);
    QStringList jsonFiles = dir.entryList();
    return jsonFiles;
}

QStringList BarcodeAnalyzer::getFolderContent(const QString &filePath) {
    QDir dir(filePath);
    dir.setFilter(QDir::Files);
    QStringList fileslist = dir.entryList();
    return fileslist;
}

QStringList BarcodeAnalyzer::getFolderJsonBasenames(const QString &filePath) {
    QStringList nameFilters;
    nameFilters.append("*.json");
    cout << "getFolderJsonBasenames filepath: " << filePath.toStdString() << endl;
    QDir dir(filePath);
    dir.setFilter(QDir::Files);
    dir.setNameFilters(nameFilters);
    QFileInfoList fileInfolist = dir.entryInfoList(nameFilters, QDir::Files);
    QStringList basenames;
    cout << "json basenames: ";
    for (int i = 0; i < fileInfolist.size(); i++) {
        basenames.append(fileInfolist.at(i).completeBaseName());
        cout << basenames.at(i).toStdString() << " ";
    }
    cout << endl;

    return basenames;
}

void BarcodeAnalyzer::refreshMaterialTemplates() {

    QDir dir(templatesFolder);
    QStringList nameFilters;
    nameFilters.append("*.json");
    dir.setNameFilters(nameFilters);
    QStringList jsonFiles = dir.entryList();
    QStringList jsonFilesFullPaths;
    for (int i = 0; i < jsonFiles.size(); i++) {
        jsonFilesFullPaths.append(dir.filePath(jsonFiles[i]));
    }
    if (materialTemplates == jsonFilesFullPaths) return;
    materialTemplates = jsonFilesFullPaths;
    materialTemplatesNames = jsonFiles;
    emit materialTemplatesChanged();
    emit materialTemplatesNamesChanged();
}

QString BarcodeAnalyzer::findMatchWithinTemplates(BarcodeList &readedBarcode) {
    QStringList myTemplatesList = GetMaterialTemplates();
    QString matchedTemplateName;
    for (int i = 0; i < myTemplatesList.size(); i++) {
        BarcodeList templateBarcodeList;
        templateBarcodeList.loadFromFile(myTemplatesList[i]);
        if (readedBarcode.isfullMatch(templateBarcodeList) == true) {
            matchedTemplateName.append(templateBarcodeList.name);
            readedBarcode.setName(matchedTemplateName);
            break;
        }
    }
    return matchedTemplateName;
}

static cv::Scalar getcolor() {
    const double GOLD = 0.618033988749895;
    g_hue = fmod(g_hue + GOLD, 1.0);
    double H = g_hue; // 0..1
    double S = 0.9;   // насыщенность
    double V = 0.95;  // яркость

    double h = H * 6.0;
    int i = static_cast<int>(std::floor(h));
    double f = h - i;
    double p = V * (1.0 - S);
    double q = V * (1.0 - S * f);
    double t = V * (1.0 - S * (1.0 - f));
    double r = 0, g = 0, b = 0;
    switch (i % 6) {
    case 0:
        r = V;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = V;
        b = p;
        break;
    case 2:
        r = p;
        g = V;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = V;
        break;
    case 4:
        r = t;
        g = p;
        b = V;
        break;
    case 5:
        r = V;
        g = p;
        b = q;
        break;
    }

    // OpenCV uses B,G,R order and typically 0..255 for 8-bit images
    return cv::Scalar(std::round(b * 255.0), std::round(g * 255.0), std::round(r * 255.0));
}
