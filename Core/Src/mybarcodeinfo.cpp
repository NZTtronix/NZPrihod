#include "mybarcodeinfo.h"


void MyBarcodeInfo::set(const Barcode &bc) {

    cv::RotatedRect rect;
  width = round(sqrt(pow((bc.position()[1].x - bc.position()[0].x), 2) +
               pow((bc.position()[1].y - bc.position()[0].y), 2)));
  height = round(sqrt(pow((bc.position()[1].x - bc.position()[2].x), 2) +
                pow((bc.position()[1].y - bc.position()[2].y), 2)));
  centerX = round(abs((bc.position()[2].x + bc.position()[0].x) / 2));
  centerY = round(abs((bc.position()[0].y + bc.position()[2].y) / 2));

  angle = bc.orientation();
  area = round(width * height);
  type = ToString(bc.format());
  content = (bc.text());
}
void MyBarcodeInfo::show() {
  std::cout << "\tCenterX: " << centerX << ";";
  std::cout << "\tCenterY: " << centerY << ";";
  // cout << "\tAngle: " << angle << ";";
  std::cout << "\twidth: " << width << ";";
  std::cout << "\theight: " << height << ";";
  std::cout << "\tArea: " << area << ";";
  std::cout << "\tBarcodeType: " << type << ";";
  std::cout << "\tData: " << content << endl;
}
QString MyBarcodeInfo::ToQstring() {
  QString out = QString(
                        "x:%1, y:%2 | "
                        "w:%3, h:%4, S:%5 | "
                        "t:%6, d: %7\n")
                    .arg(centerX)
                      .arg(centerY)
                      .arg(width)
                    .arg(height)
                    .arg(area)
                    .arg(QString::fromStdString(type))
                    .arg(QString::fromStdString(content));
  return out;
}

QJsonObject MyBarcodeInfo::toJson() const
{
    QJsonObject obj;
    obj["width"] = width;
    obj["height"] = height;
    obj["centerX"] = centerX;
    obj["centerY"] = centerY;
    obj["angle"] = angle;
    obj["area"] = area;
    obj["type"] = QString::fromStdString(type);
    obj["content"] = QString::fromStdString(content);
    return obj;
}
bool MyBarcodeInfo::compare(const MyBarcodeInfo & cmpbarcode) const
{
    if (abs(width - cmpbarcode.width)/width > compareThreshold) return false;
    // if (abs(height - cmpbarcode.height)/height > compareThreshold) return false;
    // if (type != cmpbarcode.type) return false;

    return true;
}

