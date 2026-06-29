//C:\projects\c_cpp\hik\Core\Include\hikcamera.h
#pragma once


 #include "MvCameraControl.h"
#include "QObject"
// #include "hikcamera.h"

#include <cstring>
#include <qDebug>
#define MV_DEVICES_TYPES (MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE)
  

namespace mycam {
    void* captureTaskHandle = NULL; 
   void* mvHandle = NULL;
    MV_CC_DEVICE_INFO_LIST stDeviceList ;
    int nRet = MV_OK;


    int initCamera() {
        const int DEVICE_INDEX_TO_CHOOSE=0;

    do {
        nRet = MV_CC_Initialize();  if (nRet!=MV_OK) { qDebug()<<"Initialize SDK fail! nRet [0x%x]\n"<< nRet; break;} 
        //clang-format off
        nRet = MV_CC_EnumDevices(MV_DEVICES_TYPES, &stDeviceList);  if (nRet!=MV_OK) { printf("Find No Devices!\n"); break;}
       
        MV_CC_DEVICE_INFO *deviceInfo = stDeviceList.pDeviceInfo[0];
        
        nRet = MV_CC_CreateHandle(&mvHandle, stDeviceList.pDeviceInfo[0]);  if (nRet!=MV_OK) { printf("Create Handle fail! nRet [0x%x]\n", nRet);   break;   }
        
        // if (deviceInfo->nTLayerType != MV_GIGE_DEVICE) { printf("Find No Devices!\n"); break;}
        if (deviceInfo->nTLayerType != MV_GIGE_INTERFACE) { printf("Find No MV_GIGE_DEVICE Devices!\n"); ;}
       
        nRet = MV_CC_OpenDevice(mvHandle); if (nRet!=MV_OK) { printf("Open Device fail! nRet [0x%x]\n", nRet); break;}

        int nPacketSize = MV_CC_GetOptimalPacketSize(mvHandle);    if (nPacketSize < 0) { break;}  

            
        nRet = MV_CC_SetIntValueEx(mvHandle, "GevSCPSPacketSize", nPacketSize);  if (nRet!=MV_OK){   printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);} 

        nRet = MV_CC_SetEnumValue(mvHandle, "TriggerMode", 0);  if (nRet!=MV_OK){ printf("Set Trigger Mode :! nRet [0x%x]\n", nRet); break;}
        



    } while (0);
    return nRet;
};

void getoptimalpacketsize(){
    if (stDeviceList.pDeviceInfo[0]->nTLayerType != MV_GIGE_DEVICE) {
            int nPacketSize = MV_CC_GetOptimalPacketSize(mvHandle);
            if (nPacketSize > 0) {
                if (nRet!=MV_OK){   printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);}
            } else {
                printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
            }
        }
}


bool showDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo) {
    if (NULL == pstMVDevInfo) {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE) {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
        printf("CurrentIp: %d.%d.%d.%d\n", nIp1, nIp2, nIp3, nIp4);
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
    } else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE) {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
        printf("Device Number: %d\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
    } else if (pstMVDevInfo->nTLayerType == MV_GENTL_GIGE_DEVICE) {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
    } else if (pstMVDevInfo->nTLayerType == MV_GENTL_CAMERALINK_DEVICE) {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chModelName);
    } else if (pstMVDevInfo->nTLayerType == MV_GENTL_CXP_DEVICE) {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chModelName);
    } else if (pstMVDevInfo->nTLayerType == MV_GENTL_XOF_DEVICE) {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chModelName);
    } else {
        printf("Not support.\n");
    }

    return true;
}






// // ch:保存图片 | en:Save Image
// int SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType,MV_FRAME_OUT* stImageInfo)
// {
// 	char chImageName[256] = {0};
// 	MV_CC_IMAGE stImg;
// 	MV_CC_SAVE_IMAGE_PARAM stSaveParams;
// 	memset(&stSaveParams, 0, sizeof(MV_CC_SAVE_IMAGE_PARAM));
// 	memset(&stImg, 0, sizeof(MV_CC_SAVE_IMAGE_PARAM));

// 	stImg.enPixelType = stImageInfo->stFrameInfo.enPixelType;
// 	stImg.nHeight = stImageInfo->stFrameInfo.nExtendHeight; 
// 	stImg.nWidth = stImageInfo->stFrameInfo.nExtendWidth;
// 	stImg.nImageLen = stImageInfo->stFrameInfo.nFrameLenEx;
// 	stImg.pImageBuf = stImageInfo->pBufAddr;

// 	stSaveParams.enImageType = enSaveImageType;
// 	stSaveParams.iMethodValue = 1;
// 	stSaveParams.nQuality = 99;
//     stSaveParams.nEndian = 0;       //保存TIFF图像时的字节序 1-大端存储  0、其他值-小端存储
	
// 	if (MV_Image_Bmp == stSaveParams.enImageType)
// 	{
// 		sprintf_s(chImageName, 256, "Image_w%d_h%d_fn%03d.bmp", stImageInfo->stFrameInfo.nExtendWidth,stImageInfo->stFrameInfo.nExtendHeight, stImageInfo->stFrameInfo.nFrameNum);
// 	}
// 	else if (MV_Image_Jpeg == stSaveParams.enImageType)
// 	{
// 		sprintf_s(chImageName, 256, "Image_w%d_h%d_fn%03d.jpg", stImageInfo->stFrameInfo.nExtendWidth,stImageInfo->stFrameInfo.nExtendHeight, stImageInfo->stFrameInfo.nFrameNum);
// 	}
// 	else if (MV_Image_Tif == stSaveParams.enImageType)
// 	{
// 		sprintf_s(chImageName, 256, "Image_w%d_h%d_fn%03d.tif", stImageInfo->stFrameInfo.nExtendWidth,stImageInfo->stFrameInfo.nExtendHeight, stImageInfo->stFrameInfo.nFrameNum);
// 	}
// 	else if (MV_Image_Png == stSaveParams.enImageType)
// 	{
// 		sprintf_s(chImageName, 256, "Image_w%d_h%d_fn%03d.png", stImageInfo->stFrameInfo.nExtendWidth,stImageInfo->stFrameInfo.nExtendHeight, stImageInfo->stFrameInfo.nFrameNum);
// 	}

// 	int nRet = MV_CC_SaveImageToFileEx2(handle,&stImg, &stSaveParams, (char*)chImageName);
	
// 	return nRet;
// }

// void beginCapture() {
//     auto *t = new WorkerThread(mvHandle, this);
//     connect(t, &WorkerThread::resultReady, this, &MyCameraClass::handleResults);
//     connect(t, &QThread::finished, t, &QObject::deleteLater);
//     t->start();
// }






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



// int initCamera1(const int DEVICE_INDEX_TO_CHOOSE=0) {
//     using namespace std;
//     int nRet = MV_OK;
//     do {

//         // 1. INIT
//         nRet = MV_CC_Initialize();
//         // if (MV_OK != nRet) {
//         //     printf("Initialize SDK fail! nRet [0x%x]\n", nRet);
//         //     break;
//         // }
//         // 2. FIND ALL DEVICES FROM HIK
        
//         MV_CC_DEVICE_INFO_LIST stDeviceList;
//         memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
//         nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE, &stDeviceList);
//         if (MV_OK != nRet) {
//             printf("Enum Devices fail! nRet [0x%x]\n", nRet);
//             break;
//         }
//         // 3. SHOW LIST OF DEVICES
//         if (stDeviceList.nDeviceNum > 0) {
//             for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++) {
//                 printf("[device %d]:\n", i);
//                 MV_CC_DEVICE_INFO *pDeviceInfo = stDeviceList.pDeviceInfo[i];
//                 if (NULL == pDeviceInfo) {
//                     break;
//                 }
//                 showDeviceInfo(pDeviceInfo);
//             }
//         } else {
//             printf("Find No Devices!\n");
//             break;
//         }
//         // 4. CREATE HANDLE OF CAMERA
//         nRet = MV_CC_CreateHandle(&mvHandle, stDeviceList.pDeviceInfo[DEVICE_INDEX_TO_CHOOSE]);
//         if (MV_OK != nRet) {
//             printf("Create Handle fail! nRet [0x%x]\n", nRet);
//             break;
//         }
//         // 5. OPEN CAMERA
//         nRet = MV_CC_OpenDevice(mvHandle);
//         if (MV_OK != nRet) {
//             printf("Open Device fail! nRet [0x%x]\n", nRet);
//             break;
//         }
//         // 6. SET SOME PROPERTIES
//         // Detection network optimal package size(It only works for the GigE camera)
//         if (stDeviceList.pDeviceInfo[DEVICE_INDEX_TO_CHOOSE]->nTLayerType == MV_GIGE_DEVICE) {
//             int nPacketSize = MV_CC_GetOptimalPacketSize(mvHandle);
//             if (nPacketSize > 0) {
//                 nRet = MV_CC_SetIntValueEx(mvHandle, "GevSCPSPacketSize", nPacketSize);
//                 if (nRet != MV_OK){                    printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
//                 }
//             } else {
//                 printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
//             }
//         }

//         // 7. Set trigger mode as off
//         nRet = MV_CC_SetEnumValue(mvHandle, "TriggerMode", 0);
//         if (MV_OK != nRet) {
//             printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
//             break;
//         }

//     } while (0);
//     return nRet;
// };




