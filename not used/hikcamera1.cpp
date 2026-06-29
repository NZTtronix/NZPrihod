#include "hikcamera1.h"
#include "QDebug.h"
#include <iostream>

int MyCameraClass::initializeCamera() {
    using namespace std;
    int nRet = MV_OK;
    unsigned int uret = 0;
    const int DEVICE_INDEX_TO_CHOOSE = 0;

    do {

        // 1. INIT
        nRet = MV_CC_Initialize();
        if (MV_OK != nRet) {
            printf("Initialize SDK fail! nRet [0x%x]\n", nRet);
            break;
        }
        // 2. FIND ALL DEVICES FROM HIK
        MV_CC_DEVICE_INFO_LIST stDeviceList;
        memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
        nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE, &stDeviceList);
        if (MV_OK != nRet) {
            printf("Enum Devices fail! nRet [0x%x]\n", nRet);
            break;
        }
        // 3. SHOW LIST OF DEVICES
        if (stDeviceList.nDeviceNum > 0) {
            for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++) {
                printf("[device %d]:\n", i);
                MV_CC_DEVICE_INFO *pDeviceInfo = stDeviceList.pDeviceInfo[i];
                if (NULL == pDeviceInfo) {
                    break;
                }
                PrintDeviceInfo(pDeviceInfo);
            }
        } else {
            printf("Find No Devices!\n");
            break;
        }
        // 4. CREATE HANDLE OF CAMERA
        nRet = MV_CC_CreateHandle(&cameraHandle, stDeviceList.pDeviceInfo[DEVICE_INDEX_TO_CHOOSE]);
        if (MV_OK != nRet) {
            printf("Create Handle fail! nRet [0x%x]\n", nRet);
            break;
        }
        // 5. OPEN CAMERA
        nRet = MV_CC_OpenDevice(cameraHandle);
        if (MV_OK != nRet) {
            printf("Open Device fail! nRet [0x%x]\n", nRet);
            break;
        }
        // 6. SET SOME PROPERTIES
        // Detection network optimal package size(It only works for the GigE camera)
        if (stDeviceList.pDeviceInfo[DEVICE_INDEX_TO_CHOOSE]->nTLayerType == MV_GIGE_DEVICE) {
            int nPacketSize = MV_CC_GetOptimalPacketSize(cameraHandle);
            if (nPacketSize > 0) {
                nRet = MV_CC_SetIntValueEx(cameraHandle, "GevSCPSPacketSize", nPacketSize);
                if (nRet != MV_OK) {
                    printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
                }
            } else {
                printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
            }
        }

        // 7. Set trigger mode as off
        nRet = MV_CC_SetEnumValue(cameraHandle, "TriggerMode", 0);
        if (MV_OK != nRet) {
            printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
            break;
        }

    } while (0);
    return nRet;
};

bool MyCameraClass::PrintDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo) {
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

