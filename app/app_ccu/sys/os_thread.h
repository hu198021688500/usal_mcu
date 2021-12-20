/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件: fun
作者:   km
说明:   功能函数.
***************************************************************************************************/
#ifndef __OS_THREAD_H__                     
#define __OS_THREAD_H__
#include <ofc_object.h>
#include <kfc_task.h>

typedef struct TAppccuTaskTag TAppccuTask;
struct TAppccuTaskTag
{
PROTECTED
    TKfcTask parent_;                ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数   
};

extern TAppccuTask bmsTaskHandle;
extern TAppccuTask chargeTaskHandle;
extern TAppccuTask printTaskHandle;
extern TAppccuTask modTaskHandle;
extern TAppccuTask can3TaskHandle;
//extern TAppccuTask recordTaskHandle;
//extern TAppccuTask meterTaskHandle;
//extern TAppccuTask usbTaskHandle;

void StartBMSTask(TOfObject *object);
void StartChargeTask(TOfObject *object);
void StartPrintTask(TOfObject *object);
void StartModTask(TOfObject *object);
void StartCan3Task(TOfObject *object);
//void StartFlashTask(void const * argument);
//void StartMeterTask(void const * argument);
//void StartUsbTask(void const * argument);

void StartBMSTaskCreate(TAppccuTask *self);
void StartChargeTaskCreate(TAppccuTask *self);
void StartPrintTaskCreate(TAppccuTask *self);
void StartModTaskCreate(TAppccuTask *self);
void StartTcuCanTaskCreate(TAppccuTask *self);


#endif  //__CFUN_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/


