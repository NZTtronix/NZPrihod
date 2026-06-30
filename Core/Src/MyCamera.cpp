#include "MyCamera.h"

#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#define MV_DEVICES_TYPES (MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE)

static void __stdcall ImageCallbackEx2(MV_FRAME_OUT *pstFrame, void *pUser, bool bAutoFree);
static std::string nowString();

int MyCamera::initCamera() {
    int nRet = MV_OK;

    const int DEVICE_INDEX_TO_CHOOSE = 0;

    do {
        nRet = MV_CC_Initialize();
        if (nRet != MV_OK) {
            qDebug() << "Initialize SDK fail! nRet [0x%x]\n" << nRet;
            break;
        }
        //clang-format off
        nRet = MV_CC_EnumDevices(MV_DEVICES_TYPES, &m_stDeviceInfoList);
        if (nRet != MV_OK) {
            printf("Find No Devices!\n");
            break;
        }

        MV_CC_DEVICE_INFO *deviceInfo = m_stDeviceInfoList.pDeviceInfo[0];
        showDeviceInfo(deviceInfo);

        nRet = MV_CC_CreateHandle(&mvHandle, m_stDeviceInfoList.pDeviceInfo[0]);
        if (nRet != MV_OK) {
            printf("Create Handle fail! nRet [0x%x]\n", nRet);
            break;
        }

        if (deviceInfo->nTLayerType != MV_GIGE_INTERFACE) {
            printf("Find No MV_GIGE_DEVICE Devices!\n");
            
        }

        nRet = MV_CC_OpenDevice(mvHandle);
        if (nRet != MV_OK) {
            printf("Open Device fail! nRet [0x%x]\n", nRet);
            break;
        }
        if (m_stDeviceInfoList.pDeviceInfo[m_deviceindex]->nTLayerType == MV_GIGE_DEVICE) {
            int nPacketSize = MV_CC_GetOptimalPacketSize(mvHandle);
            if (nPacketSize > 0) {
                if (MV_CC_SetIntValueEx(mvHandle, "GevSCPSPacketSize", nPacketSize) != MV_OK) {
                    printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
                }
            } else {
                printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
            }
        }
        
        nRet = MV_CC_SetEnumValue(mvHandle, "TriggerMode", 0);
        if (nRet != MV_OK) {
            printf("Set enum Value fauled! nRet [0x%x]\n", nRet);
            break;
        } else {
            // printf("cant set trigger mode: 1 successfully! nRet [0x%x]\n", nRet);
        };
        nRet = MV_CC_RegisterImageCallBackEx2(mvHandle, ImageCallbackEx2, this, false);

        nRet = MV_CC_GetPayloadSize(mvHandle, &m_payloadlength, &m_bytesalignment);

        printf("payload[%d], bytesalignment[%x]\n", m_payloadlength, m_bytesalignment);
        m_lastimagedatabuf = static_cast<uint8_t *>(calloc(m_payloadlength * 3, sizeof(uint8_t)));
    
    
        int nRet = MV_CC_StartGrabbing(mvHandle);
    if (nRet != MV_OK) {
        printf("cant Start grabbing!!!!\n");
    } else {
        printf("STARTED grabbing!!!!\n");
    }

        
    } while (0);
    return nRet;
};

int MyCamera::onStartGrabbing() {
    int nRet = MV_CC_StartGrabbing(mvHandle);
    if (nRet != MV_OK) {
        printf("cant Start grabbing!!!!\n");
    } else {
        printf("STARTED grabbing!!!!\n");
    }
    return nRet;
}
int MyCamera::onImageRequestSignal() {
    printf("onImageRequestSignal IS CALLED !!!!!!");
    int nRet = MV_OK;
    nRet = MV_CC_TriggerSoftwareExecute(mvHandle);
    return nRet;
};

bool MyCamera::showDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo) {
    if (NULL == pstMVDevInfo) {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE) {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user
        // defined name
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

int MyCamera::checkoptimalpacketsize() {
    int nRet = MV_OK;
    if (m_stDeviceInfoList.pDeviceInfo[m_deviceindex]->nTLayerType == MV_GIGE_DEVICE) {
        int nPacketSize = MV_CC_GetOptimalPacketSize(mvHandle);
        if (nPacketSize > 0) {
            if (MV_CC_SetIntValueEx(mvHandle, "GevSCPSPacketSize", nPacketSize) != MV_OK) {
                printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
            }
        } else {
            printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
        }
    }
    return nRet;
}

void __stdcall MyCamera::ImageCallbackEx2(MV_FRAME_OUT *pstFrame, void *pUser, bool bAutoFree) {
    if (pstFrame) {
        MyCamera *cam = static_cast<MyCamera *>(pUser);
        uint32_t nFrameNum = pstFrame->stFrameInfo.nFrameNum;
        uint32_t Width = pstFrame->stFrameInfo.nExtendWidth;
        uint32_t Height = pstFrame->stFrameInfo.nExtendHeight;
        uint64_t nFrameLenEx = pstFrame->stFrameInfo.nFrameLenEx;
        int32_t enPixelType = pstFrame->stFrameInfo.enPixelType;
        uint8_t *pData = pstFrame->pBufAddr;
#if debugprintting

        printf("Frame[%d]: Width[%d], Height[%d], nFrameLenEx[%d],enPixelType[%d], at[%p] -> at'[%p]\n",
               nFrameNum,
               Width,
               Height,
               nFrameLenEx,
               enPixelType,
               pData,
               pData);
#endif
        MV_SAVE_IMAGE_PARAM_EX3 stSaveParam;
        memset(&stSaveParam, 0, sizeof(MV_SAVE_IMAGE_PARAM_EX3));
        stSaveParam.pData = pData;
        stSaveParam.nDataLen = nFrameLenEx;
        stSaveParam.enPixelType = PixelType_Gvsp_Mono8;
        stSaveParam.nHeight = Height;
        stSaveParam.nWidth = Width;
        stSaveParam.pImageBuffer = cam->m_lastimagedatabuf;
        stSaveParam.nImageLen = cam->m_lastimagelen;
        stSaveParam.nBufferSize = cam->m_payloadlength * 3;
        stSaveParam.enImageType = MV_Image_Bmp;

        int nRet = MV_CC_SaveImageEx3(cam->mvHandle, &stSaveParam);
        if (nRet != MV_OK) {
            printf("CALLBACK PIZDEC %d", nRet);
        };

        MV_CC_FreeImageBuffer(cam->mvHandle, pstFrame);

        {
            std::lock_guard<std::mutex> lock(cam->mtx);
            memcpy(cam->m_lastimagedatabuf, stSaveParam.pImageBuffer, stSaveParam.nImageLen);
            cam->m_lastimagelen = static_cast<int>(stSaveParam.nImageLen);
        }
        cam->handleNewFrame();

        // emit cam->newFrameReady();
    }
}

 QString MyCamera::nowString()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);

    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return QString::fromStdString(oss.str());
}

QImage MyCamera::getCurrentImage() {
    std::lock_guard<std::mutex> lock(mtximg);

#if debugprintting

    printf("MyCamera::getCurrentImage is called");
#endif
    return m_currentImage;
}

void MyCamera::handleNewFrame() {
    uint8_t *local;
    uint32_t localImagelen;
#if debugprintting
    printf("handleNewFrame called");
#endif
    {
        std::lock_guard<std::mutex> lock(mtx);

        localImagelen = m_lastimagelen;

        local = (uint8_t *)malloc(localImagelen);

        memcpy(local, m_lastimagedatabuf, m_lastimagelen);
    }

    {
        std::lock_guard<std::mutex> lock(mtximg);
        m_currentImage.loadFromData(local, m_lastimagelen, "BMP");
    }
    if (local != NULL) free(local);

    emit imageReady();
}

// static void __stdcall ImageCallbackEx2OLD(MV_FRAME_OUT *pstFrame, void *pUser, bool bAutoFree) {
//     if (pstFrame) {
//         MyCamera *cam = static_cast<MyCamera *>(pUser);
//         uint32_t nFrameNum = pstFrame->stFrameInfo.nFrameNum;
//         uint32_t Width = pstFrame->stFrameInfo.nExtendWidth;
//         uint32_t Height = pstFrame->stFrameInfo.nExtendHeight;
//         uint64_t nFrameLenEx = pstFrame->stFrameInfo.nFrameLenEx;
//         int32_t enPixelType = pstFrame->stFrameInfo.enPixelType;
//         uint8_t *pData = pstFrame->pBufAddr;
//         uint8_t *dataBuffer = static_cast<uint8_t *>(MV_CC_AllocAlignedBuffer(nFrameLenEx, cam->m_bytesalignment));
//         memcpy(dataBuffer, pData, nFrameLenEx);
//         cam->mvHandle;
//         printf("Frame[%d]: Width[%d], Height[%d], nFrameLenEx[%d],enPixelType[%d], at[%p] -> at'[%p]\n",
//                nFrameNum,
//                Width,
//                Height,
//                nFrameLenEx,
//                enPixelType,
//                pData,
//                dataBuffer);
//         MV_CC_FreeImageBuffer(cam->mvHandle, pstFrame);
//         char chImageName[256] = {0};
//         MV_CC_IMAGE stImg;
//         MV_CC_SAVE_IMAGE_PARAM stSaveParams;
//         memset(&stSaveParams, 0, sizeof(MV_CC_SAVE_IMAGE_PARAM));
//         memset(&stImg, 0, sizeof(MV_CC_SAVE_IMAGE_PARAM));
//         stImg.enPixelType = PixelType_Gvsp_Mono8;
//         stImg.nHeight = Height;
//         stImg.nWidth = Width;
//         stImg.nImageLen = nFrameLenEx;
//         stImg.pImageBuf = dataBuffer;
//         stSaveParams.enImageType = MV_Image_Bmp;
//         stSaveParams.iMethodValue = 1;
//         stSaveParams.nQuality = 99;
//         stSaveParams.nEndian = 0;
//         std::string fullpath = "C:/projects/qt/QmlAppTest/appdata/History/" + nowString() + ".bmp";
//         std::cout << "full path to save the pic: " << fullpath << std::endl;
//         int nRet = MV_CC_SaveImageToFileEx2(cam->mvHandle, &stImg, &stSaveParams, fullpath.c_str());
//         if (nRet != MV_OK) {
//         };
//         QString url = "file:///" + QString::fromLocal8Bit(fullpath.c_str());
//         MV_CC_FreeAlignedBuffer(dataBuffer);
//         QMetaObject::invokeMethod(cam, [cam, url] { emit cam->newImageSaved(url); }, Qt::QueuedConnection);
//     }
// }
