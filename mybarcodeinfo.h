#ifndef MYBARCODEINFO_H
#define MYBARCODEINFO_H

#include <QString>
#include <QJsonObject>
#include <iostream>
#include <ZXing/ReadBarcode.h>
#include <ZXing/Barcode.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace ZXing;

namespace {
struct Pos { double x = 0, y = 0; };
static const double compareThreshold = 0.1;

}









struct MyBarcodeInfo
{

public:
     MyBarcodeInfo() = default;
    // MyBarcodeInfo(const Barcode & bc);
    ~MyBarcodeInfo(){};
     int localNum;
    double width=0;
    double height=0;
    double centerX=0;
    double centerY=0;
    double angle=0;
    double area=0;
    string type;
    string content;

    int cutfromstart;
    int cutfromend;

    string stringToPrint;

    void set(const Barcode & bc);
    void setnum(int num) {localNum = num;}
    void show();
    QString ToQstring();
    bool compare(const MyBarcodeInfo &) const;
    QJsonObject toJson() const;
    void setfromJson(const QJsonObject &obj)
    {
        if (obj.contains("width"))   width   = obj["width"].toDouble();
        if (obj.contains("height"))  height  = obj["height"].toDouble();
        if (obj.contains("centerX")) centerX = obj["centerX"].toDouble();
        if (obj.contains("centerY")) centerY = obj["centerY"].toDouble();
        if (obj.contains("angle"))   angle   = obj["angle"].toDouble();
        if (obj.contains("area"))    area    = obj["area"].toDouble();
        if (obj.contains("type"))    type    = obj["type"].toString().toStdString();
        if (obj.contains("content")) content = obj["content"].toString().toStdString();
    }
private:
};


#endif // MYBARCODEINFO_H
