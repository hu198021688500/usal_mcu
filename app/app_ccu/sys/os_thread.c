/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技
文件:   os_thread
作者:   km
说明:   OS thread.
***************************************************************************************************/

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */ 
#include "os_thread.h"
#include "charge_task.h"
#include "bms_task.h"
#include "tick_handle.h"
#include "sys_type.h"
#include "log.h"
#include "time_cur.h"
#include "kfc_timer.h"
#include "kfc_system.h"
#include "io_megg.h"
#include "bms_device.h"
#include "modyfy.h"
#include "cantcu.h"


/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
TAppccuTask bmsTaskHandle;
TAppccuTask chargeTaskHandle;
TAppccuTask printTaskHandle;
TAppccuTask modTaskHandle;
TAppccuTask can3TaskHandle;
//TAppccuTask recordTaskHandle;
//TAppccuTask meterTaskHandle;
//TAppccuTask usbTaskHandle;


/* USER CODE BEGIN Header_StartBMSTask */
/**
* @brief Function implementing the bmsTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBMSTask */
void StartBMSTask(TOfObject *object)
{
  /* USER CODE BEGIN StartBMSTask */
  /* Infinite loop */
  TAppccuTask *self = (TAppccuTask *)object;
  BMSTaskInit();
  while (!TKfcTaskTerminated((TKfcTask *)self))
  {
      BMSTaskMain();
      KfcSleep(1);
  }
  /* USER CODE END StartBMSTask */
}

void StartBMSTaskCreate(TAppccuTask *self)
{
#if defined (__GNUC__)
    static char stack[512] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[1024];
#endif

    TKfcTaskCreate(&self->parent_, kKfcTaskPriority8, stack, sizeof(stack));
    //OVERRIDE_METHOD(TOfObject, Destroy, TTestTaskDestroy);
    //self->parent_.OnCreate = TTestTaskOnTaskCreate;
    self->parent_.OnExecute = StartBMSTask;
    
    TKfcTaskResume(&self->parent_);
}

/* USER CODE BEGIN Header_StartChargeTask */
/**
* @brief Function implementing the chargeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartChargeTask */
void StartChargeTask(TOfObject *object)
{
  /* USER CODE BEGIN StartChargeTask */
  /* Infinite loop */
  TAppccuTask *self = (TAppccuTask *)object;
  chargeTaskInit();
  while (!TKfcTaskTerminated((TKfcTask *)self))
  {
    charger_task_main();
    KfcSleep(1);
  }
  /* USER CODE END StartChargeTask */
}

void StartChargeTaskCreate(TAppccuTask *self)
{
#if defined (__GNUC__)
    static char stack[521] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[1024];
#endif

    TKfcTaskCreate(&self->parent_, kKfcTaskPriority8, stack, sizeof(stack));

    self->parent_.OnExecute = StartChargeTask;
    
    TKfcTaskResume(&self->parent_);
}

/* USER CODE BEGIN Header_StartPrintTask */
/**
* @brief Function implementing the printTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPrintTask */

void StartPrintTask(TOfObject *object)
{
  /* USER CODE BEGIN StartPrintTask */
  /* Infinite loop */
  uint16_t time_5s_flag = 0;
  uint16_t time_60s_flag = 0;
  uint8_t one_time_flag = 0;
  /* Infinite loop */

  TAppccuTask *self = (TAppccuTask *)object;
  devOpenAllInit(self);
  while (!TKfcTaskTerminated((TKfcTask *)self))
  {
    /*if(uart5_state()==BOOL_TRUE)//(++u5_cnt>=3)//115200/8/1000=14.4,1ms发送14.4
    {
        UART5_Transmit();
        
    }*/
	time_5s_flag++;
	if(time_5s_flag >= 2000)
    {
    	time_5s_flag = 0;
		//DLOG("--heart[%d]--",adcTestTemperatureOpt);
		//DLOG("[%d][%d][%d]",sSysMegg[0].v_Gun,sSysMegg[1].v_Gun,sSysMegg[0].hz_10x);
	    //DLOG("[%d][%d]",sSysMegg[0].v_CC1_10x,sSysMegg[1].v_CC1_10x);
	    //DLOG("[%d][%d]",chargerMegg[0].unStep.bit.bworking,get_charge_state(1));
		//DLOG("[%d][%d][%d]",bmsDevice[1].frameData.brmData.brmBatteryType,bmsDevice[1].frameData.bclData.bclVoltageDemand,bmsDevice[1].frameData.bclData.bclCurrentDemand);
	    //DLOG("[%d~%d][%d~%d][%d~%d]",sSysMegg[0].v_Gun,sSysMegg[1].v_Gun,sSysMegg[0].v_GFD_P,sSysMegg[0].v_GFD_N,sSysMegg[1].v_GFD_P,sSysMegg[1].v_GFD_N);
	    DLOG("%d[%d~%d]",sSysMegg[0].hz_10x,sSysMegg[0].t_Inlet,sSysMegg[0].t_Outlet);
    }

	#if 1
	time_60s_flag++;
	if(time_60s_flag >= 12000)
	{
		time_60s_flag = 0;
	}
	if((one_time_flag == 0)&&(time_60s_flag>10000))
	{
		one_time_flag = 1;
		//start_charger_interface(START_BY_TCU,0);
	
	}
	#endif
	
	
	
    KfcSleep(1);
  }
  /* USER CODE END StartPrintTask */
}          

void StartPrintTaskCreate(TAppccuTask *self)
{
#if defined (__GNUC__)
    static char stack[512] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[512];
#endif

    TKfcTaskCreate(&self->parent_, kKfcTaskPriority2, stack, sizeof(stack));

    self->parent_.OnExecute = StartPrintTask;
    
    TKfcTaskResume(&self->parent_);
}

/* USER CODE BEGIN Header_StartModTask */
/**
* @brief Function implementing the modTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartModTask */
void StartModTask(TOfObject *object)
{
  /* USER CODE BEGIN StartModTask */

  /* Infinite loop */
  TAppccuTask *self = (TAppccuTask *)object;
  while (!TKfcTaskTerminated((TKfcTask *)self))
  {
    Project_Type_Deal();
	switch(sSysConfig.sInt.uModuleType)
	{
		case MOD_TYPE_INFY_30KW:
			yfy_mod_Com_task();
			break;

		case MOD_TYPE_WINLINE_30KW:
			
			break;

		
		default:
			break;
	}
		
    KfcSleep(1);
  }
  /* USER CODE END StartModTask */
}

void StartModTaskCreate(TAppccuTask *self)
{
#if defined (__GNUC__)
    static char stack[512] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[512];
#endif

    TKfcTaskCreate(&self->parent_, kKfcTaskPriority6, stack, sizeof(stack));

    self->parent_.OnExecute = StartModTask;
    
    TKfcTaskResume(&self->parent_);
}

/* USER CODE BEGIN Header_StartCan3Task */
/**
* @brief Function implementing the can3Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCan3Task */
void StartTcuCanTask(TOfObject *object)
{
  /* USER CODE BEGIN StartCan3Task */
  /* Infinite loop */
  TAppccuTask *self = (TAppccuTask *)object;
  while (!TKfcTaskTerminated((TKfcTask *)self))
  {
    tcu_com_handle();
    KfcSleep(1);
  }
  /* USER CODE END StartCan3Task */
}

void StartTcuCanTaskCreate(TAppccuTask *self)
{
#if defined (__GNUC__)
    static char stack[512] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[512];
#endif

    TKfcTaskCreate(&self->parent_, kKfcTaskPriority6, stack, sizeof(stack));

    self->parent_.OnExecute = StartTcuCanTask;
    
    TKfcTaskResume(&self->parent_);
}

#if 0
/* USER CODE BEGIN Header_StartRecord */
/**
* @brief Function implementing the flashTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRecord */
void StartFlashTask(TOfObject *object)
{
  /* USER CODE BEGIN StartFlashTask */
  /* Infinite loop */
  TAppccuTask *self = (TAppccuTask *)object;
  while (!TKfcTaskTerminated((TKfcTask *)self))
  {
    KfcSleep(1);
  }
  /* USER CODE END StartFlashTask */
}

void StartFlashTaskCreate(TAppccuTask *self)
{
#if defined (__GNUC__)
    static char stack[512] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[512];
#endif

    TKfcTaskCreate(&self->parent_, kKfcTaskPriority6, stack, sizeof(stack));

    self->parent_.OnExecute = StartFlashTask;
    
    TKfcTaskResume(&self->parent_);
}

/* USER CODE BEGIN Header_StartMeterTask */
/**
* @brief Function implementing the meterTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMeterTask */
void StartMeterTask(TOfObject *object)
{
  /* USER CODE BEGIN StartMeterTask */
  /* Infinite loop */
  TAppccuTask *self = (TAppccuTask *)object;
  while (!TKfcTaskTerminated((TKfcTask *)self))
  {
    KfcSleep(1);
  }
  /* USER CODE END StartMeterTask */
}

void StartMeterTaskCreate(TAppccuTask *self)
{
#if defined (__GNUC__)
    static char stack[512] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[512];
#endif

    TKfcTaskCreate(&self->parent_, kKfcTaskPriority6, stack, sizeof(stack));

    self->parent_.OnExecute = StartMeterTask;
    
    TKfcTaskResume(&self->parent_);
}

/* USER CODE BEGIN Header_StartUsbTask */
/**
* @brief Function implementing the usbTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsbTask */
void StartUsbTask(void const * argument)
{

  /* USER CODE BEGIN StartUsbTask */
  /* Infinite loop */
  for(;;)
  {
    usb_main();
    KfcSleep(1);
  }
  /* USER CODE END StartUsbTask */
}
#endif

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
