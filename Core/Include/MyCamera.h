#ifndef MYCAMERA_H
#define MYCAMERA_H


#include "MvCameraControl.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <thread>
#include <mutex> 
#include <QImage>

class MyCamera : public QObject {
    Q_OBJECT

  public:
    explicit MyCamera(QObject *parent = nullptr) : QObject(parent) {
    }
   
    Q_INVOKABLE int initCamera();
    Q_INVOKABLE int setTriggerMode() {
        m_triggerenable = MV_TRIGGER_MODE_ON ; 
        return MV_CC_SetTriggerMode(mvHandle, MV_TRIGGER_MODE_ON);
    }
    Q_INVOKABLE int resetTriggerMode() {
        m_triggerenable = MV_TRIGGER_MODE_OFF ; 
        return MV_CC_SetTriggerMode(mvHandle, MV_TRIGGER_MODE_OFF); 
    }
    Q_INVOKABLE int getTriggerMode() {
        MVCC_ENUMVALUE pstValue;
        memset(&pstValue, 0, sizeof(pstValue));
        MV_CC_GetTriggerMode(mvHandle, &pstValue);
        return pstValue.nCurValue;
    }
    Q_INVOKABLE int GetEnumValue(QString key) {
        int value = 0;
        MVCC_ENUMVALUE_EX readValue;
        std::string keystd = key.toStdString();
        MV_CC_GetEnumValueEx(mvHandle,keystd.c_str(), &readValue);
        value = readValue.nCurValue;
        return value;
    }
    Q_INVOKABLE bool saveImage() {
        QString fullpath = "C:/projects/qt/QmlAppTest/appdata/History/" + nowString() + ".bmp";
        qDebug() << "path = " << fullpath;
        QImage img = getCurrentImage();
        if (img.isNull()) return false;
        bool res = img.save(fullpath) ;
        qDebug()<< "saved? =" <<res;
        m_lastsavedpath = fullpath;
        return res;
    }
    Q_INVOKABLE QString getLastSavedPath(){
        return m_lastsavedpath;
    }
    static QString nowString();
    
    uint64_t m_payloadlength = 0;
    unsigned int m_bytesalignment = 0;
    MV_CC_DEVICE_INFO_LIST m_stDeviceInfoList;
    void *captureTaskHandle = NULL;
    void *mvHandle = NULL;
    void setDeviceIndex(int idx){m_deviceindex = idx;} 
    QImage getCurrentImage();
  signals:
    void newImageSaved(const QString &fileUrl);
    // void newFrameReady();
    void imageReady();
    
  public slots:
    int onImageRequestSignal();
    int onStartGrabbing();
  private:

    static void __stdcall ImageCallbackEx2(MV_FRAME_OUT *pstFrame, void *pUser, bool bAutoFree);
    void handleNewFrame();
    bool showDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo);
    int checkoptimalpacketsize();
    
    std::mutex mtx, mtximg;
    uint8_t *m_lastimagedatabuf;
    uint32_t m_lastimagelen;
    QImage m_currentImage;
    int m_triggerenable;
    int m_deviceindex = 0;
    int m_NeedToSaveImage = 0;
    QString m_lastsavedpath;

    
};

#endif



// class MyCameraGrabber : public QObject {
//     Q_OBJECT

//   public:
//     explicit MyCamera(QObject *parent = nullptr) : QObject(parent) {}
   
//     Q_INVOKABLE int initCamera();
//     uint64_t m_payloadlength = 0;
//     unsigned int m_bytesalignment = 0;
//     MV_CC_DEVICE_INFO_LIST m_stDeviceInfoList;
//     void *captureTaskHandle = NULL;
//     void *mvHandle = NULL;
//     void setDeviceIndex(int idx){m_deviceindex = idx;} 

//   signals:
//     void newImageSaved(const QString &fileUrl);

//   public slots:
//     int onImageRequestSignal();

//   private:
//     uint8_t *m_dataPtr;

//     int m_deviceindex = 0;
//     bool showDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo);
//     int checkoptimalpacketsize();
// };























// static int SaveRawToFile(QString fullPath, MV_FRAME_OUT &stImageInfo) {
//     char str[256] = {0};
//     memcpy(str, fullPath.toStdString().c_str(), strlen(fullPath.toStdString().c_str()));

//     sprintf_s(
//         str, 256, "Image_w%d_h%d_fn%03d.raw", stImageInfo.stFrameInfo.nExtendWidth, stImageInfo.stFrameInfo.nExtendHeight, stImageInfo.stFrameInfo.nFrameNum);
//     FILE *file = fopen(str, "wb+");
//     if (NULL != file) {
//         size_t bytesWritten = fwrite(stImageInfo.pBufAddr, 1, stImageInfo.stFrameInfo.nFrameLenEx, file);
//         if (stImageInfo.stFrameInfo.nFrameLenEx == bytesWritten) {
//             printf("Save image raw success.\n");
//         } else {
//             printf("Save image raw failed.\n");
//         }
//         fclose(file);
//     }
//     return MV_OK;
// };