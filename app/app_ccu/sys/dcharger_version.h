/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   dcharger_version
作者:   km
说明:   版本信息.
***************************************************************************************************/

#ifndef __DCHARGER_VERSION_H
#define __DCHARGER_VERSION_H



#define CLIENT_STANDARD             0                           //标准V6










#define HOTTEST                                                 //正式版本注释掉，热测试
//#define CHECK_VERSION                                           //正式版本注释掉，工装版本使能
//#define INSDET_VERSION 100                                    //使用单独绝缘板子需定义
//#define OLD_VERSION                                           //第一版硬件
//#define METER_ENABLE                                          //是否使能电表功能--使能将开关打开



#define DCHARGER_CLIENT_TYPE               CLIENT_STANDARD         //客户类型
//非标参数保存，赋值给sSysConfig.sUnstandard即可

#define DCHARGER_VERSION                    100                     //主版本号

#define DCHARGER_VERSION_BRANCH             00                      //子版本号,出厂版本号必须为偶数



#endif

/***************************************************************************************************
文件结束.
***************************************************************************************************/
