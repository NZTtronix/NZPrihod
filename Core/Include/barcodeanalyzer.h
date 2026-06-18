#ifndef BARCODEANALYZER_H
#define BARCODEANALYZER_H

#pragma once

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QTime>
#include <QUrl>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <ZXing/Barcode.h>
#include <ZXing/ReadBarcode.h>

#include "mybarcodeinfo.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include "barcodelist.h"
#include "opencv2/imgcodecs.hpp"
#include "qimage.h"

class BarcodeAnalyzer : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QStringList materialTemplates READ GetMaterialTemplates NOTIFY
                 materialTemplatesChanged)
  Q_PROPERTY(QStringList materialTemplatesNames READ GetMaterialTemplatesNames
                 NOTIFY materialTemplatesNamesChanged)
  Q_PROPERTY(QString imgPath READ getImgPath WRITE setImgPath NOTIFY imagePathChanged)

public:
      explicit BarcodeAnalyzer(QObject *parent = nullptr) : QObject(parent)
  {
  }
  Q_INVOKABLE QString processImage(const QString &fileUrlOrPath);
  Q_INVOKABLE QString processTemplateImage(QString templateName);
    Q_INVOKABLE double DetectRectangle(cv::Mat mat);

  Q_INVOKABLE void refreshMaterialTemplates();
  QString getImgPath() { return m_imgPath; }
  void setImgPath(const QString &path);
  QStringList GetMaterialTemplates() { return materialTemplates; }
  QStringList GetMaterialTemplatesNames() { return materialTemplatesNames; }
  Q_INVOKABLE QString loadJsonStringFromTemplateFile(const QString &filePath);
  Q_INVOKABLE QStringList loadAllTemlateFileNames();
  Q_INVOKABLE QStringList getFolderContent(const QString &filePath);
  Q_INVOKABLE QStringList getFolderJsonBasenames(const QString &filePath);
  Q_INVOKABLE bool addNewEmptyTemplateJson( const QString &filePath);


  Q_INVOKABLE bool copyImageToTemplatesFolderAsChosenTemplateImage(QString templateName,QUrl chosenImage);
  Q_INVOKABLE bool makeTemplateFromSrcImage(QString templateName);


Q_INVOKABLE bool saveTo(QString jsonstr,const QString &filePath) const;
  enum deskewVariant
  {
    threshold210 = 0,
    threshold220,
    threshold230,
    threshold210_e,
    threshold220_e,
    threshold230_e,
    thresholdOtsu,
    adaptive
  };
  
private:
  const char *templatesFolder = "C:/projects/qt/QmlAppTest/templates/";
  QStringList materialTemplates;
  QStringList materialTemplatesNames;
  QString templatesPath = "C:/projects/qt/QmlAppTest/templates/";
  QString findMatchWithinTemplates(BarcodeList &readedBarcode);
  QString m_imgPath;
  cv::Mat cropImageByReel(const cv::Mat &src);
  double alignImageAngle(const cv::Mat &src);
  // double alignImageAngle(const cv::Mat &src, double angle);

  void adjustExposureWithBlackLevel(const cv::Mat &src, cv::Mat &dst,
                                    float exposure, int blackLevel);
  cv::Mat detectAndDeskewBarcode(const cv::Mat &src, int variant);
  
  void shortShow(const cv::Mat &matrix);

signals:
  void materialTemplatesChanged();
  void materialTemplatesNamesChanged();
  void imagePathChanged();
};

#endif
