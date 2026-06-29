#ifndef HIKCAMERA_H
#define HIKCAMERA_H
#include "QObject"
#include <cstdint>

#include <QThread>

class WorkerThread : public QThread {
    Q_OBJECT
  public:
    explicit WorkerThread(void *  handle, QObject *parent = nullptr) : QThread(parent), cameraHandle(handle) {}

  protected:
    void run() override;
  signals:
    void resultReady( MV_FRAME_OUT OutFrame);
    private:
    void* cameraHandle;
    

};

void WorkerThread::run() {
    int nRet = MV_OK;
    MV_FRAME_OUT stOutFrame = {0};
    do {
        nRet = MV_CC_GetImageBuffer(cameraHandle, &stOutFrame, 5000);
        if (nRet == MV_OK) {
            printf("Get Image Buffer: Width[%d], Height[%d], FrameNum[%d]\n",
                   stOutFrame.stFrameInfo.nExtendWidth,
                   stOutFrame.stFrameInfo.nExtendHeight,
                   stOutFrame.stFrameInfo.nFrameNum
            );
        } else {
            printf("Get Image fail! nRet [0x%x]\n", nRet);
        } 
        
    } while(0);

    emit resultReady(stOutFrame);
}

class MyCameraClass : public QObject {
    Q_OBJECT

  public:
    void startWorkInAThread();

    MyCameraClass::MyCameraClass() {}
    void *cameraHandle = NULL;

    int initializeCamera();
    bool PrintDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo);

  signals:
    void endCapture();
  public slots:
    void handleResults() {
        MV_FRAME_OUT * pFrame = stImageInfo; 
    };

    private: 
           SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType, MV_FRAME_OUT *stImageInfo);


};


void MyCameraClass::startWorkInAThread() {
    auto *t = new WorkerThread(cameraHandle, this);
    connect(t, &WorkerThread::resultReady, this, &MyCameraClass::handleResults);
    connect(t, &QThread::finished, t, &QObject::deleteLater);
    t->start();
}

//    bool IsHBPixelFormat(MvGvspPixelType ePixelType)
//     {
//         switch (ePixelType)
//         {
//         case PixelType_Gvsp_HB_Mono8:
//         case PixelType_Gvsp_HB_Mono10:
//         case PixelType_Gvsp_HB_Mono10_Packed:
//         case PixelType_Gvsp_HB_Mono12:
//         case PixelType_Gvsp_HB_Mono12_Packed:
//         case PixelType_Gvsp_HB_Mono16:
//         case PixelType_Gvsp_HB_RGB8_Packed:
//         case PixelType_Gvsp_HB_BGR8_Packed:
//         case PixelType_Gvsp_HB_RGBA8_Packed:
//         case PixelType_Gvsp_HB_BGRA8_Packed:
//         case PixelType_Gvsp_HB_RGB16_Packed:
//         case PixelType_Gvsp_HB_BGR16_Packed:
//         case PixelType_Gvsp_HB_RGBA16_Packed:
//         case PixelType_Gvsp_HB_BGRA16_Packed:
//         case PixelType_Gvsp_HB_YUV422_Packed:
//         case PixelType_Gvsp_HB_YUV422_YUYV_Packed:
//         case PixelType_Gvsp_HB_BayerGR8:
//         case PixelType_Gvsp_HB_BayerRG8:
//         case PixelType_Gvsp_HB_BayerGB8:
//         case PixelType_Gvsp_HB_BayerBG8:
//         case PixelType_Gvsp_HB_BayerRBGG8:
//         case PixelType_Gvsp_HB_BayerGB10:
//         case PixelType_Gvsp_HB_BayerGB10_Packed:
//         case PixelType_Gvsp_HB_BayerBG10:
//         case PixelType_Gvsp_HB_BayerBG10_Packed:
//         case PixelType_Gvsp_HB_BayerRG10:
//         case PixelType_Gvsp_HB_BayerRG10_Packed:
//         case PixelType_Gvsp_HB_BayerGR10:
//         case PixelType_Gvsp_HB_BayerGR10_Packed:
//         case PixelType_Gvsp_HB_BayerGB12:
//         case PixelType_Gvsp_HB_BayerGB12_Packed:
//         case PixelType_Gvsp_HB_BayerBG12:
//         case PixelType_Gvsp_HB_BayerBG12_Packed:
//         case PixelType_Gvsp_HB_BayerRG12:
//         case PixelType_Gvsp_HB_BayerRG12_Packed:
//         case PixelType_Gvsp_HB_BayerGR12:
//         case PixelType_Gvsp_HB_BayerGR12_Packed:
//             return true;
//         default:
//             return false;
//         }
//     };

class MyCameraImageProvider {
  public:
    Q_INVOKABLE int SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType, MV_FRAME_OUT *stImageInfo);
};

#endif