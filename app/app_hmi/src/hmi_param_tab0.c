#include "hmi_param_tab0.h"
#include "kfc_srt.h"
#include "srt_config.h"

/**
 * 参数表-设备编号(只读)
 */
static THmiParamCmdResult ParamItemDevNum(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
        return kHmiParamCmdResultPermissionErr;
    else
    {
        return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-硬件版本(只读)
 */
static THmiParamCmdResult ParamItemHardwareVersion(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
        return kHmiParamCmdResultPermissionErr;
    else
    {
        return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-Boot软件版本(只读)
 */
static THmiParamCmdResult ParamItemBootSoftVersion(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
        return kHmiParamCmdResultPermissionErr;
    else
    {
        return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-App软件版本(只读)
 */
static THmiParamCmdResult ParamItemAppSoftVersion(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
        return kHmiParamCmdResultPermissionErr;
    else
    {
        return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-参数表版本号(只读)
 */
static THmiParamCmdResult ParamItemParamTabVersion(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
        return kHmiParamCmdResultPermissionErr;
    else
    {
        return kHmiParamCmdResultDoErr;
    }
}/**
 * 参数表-CAN0波特率
 */
static THmiParamCmdResult ParamItemCan0Baud(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    { 
        if (KfcSrtPhmSetU32(kSrtPhmCan0Baud, *(UInt32 *)data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU32(kSrtPhmCan0Baud, (UInt32* )data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-CAN1波特率
 */
static THmiParamCmdResult ParamItemCan1Baud(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    { 
        if (KfcSrtPhmSetU32(kSrtPhmCan1Baud, *(UInt32 *)data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU32(kSrtPhmCan1Baud, (UInt32* )data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-CAN2波特率
 */
static THmiParamCmdResult ParamItemCan2Baud(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    { 
        if (KfcSrtPhmSetU32(kSrtPhmCan2Baud, *(UInt32 *)data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU32(kSrtPhmCan2Baud, (UInt32* )data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}

/**
 * 参数表-项目类型
 */
static THmiParamCmdResult ParamItemProjectType(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    {
        if (KfcSrtPhmSetU08(kSrtPhmProjectType, *data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU08(kSrtPhmProjectType, data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-模块类型
 */
static THmiParamCmdResult ParamItemModuleType(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    {
        if (KfcSrtPhmSetU08(kSrtPhmModuleType, *data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU08(kSrtPhmModuleType, data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-模块数量 
 */
static THmiParamCmdResult ParamItemModuleCnt(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    {
        if (KfcSrtPhmSetU08(kSrtPhmModuleCnt, *data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU08(kSrtPhmModuleCnt, data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-接触器群充类型
 */
static THmiParamCmdResult ParamItemContGroupType(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    {
        if (KfcSrtPhmSetU08(kSrtPhmContGroupType, *data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU08(kSrtPhmContGroupType, data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-充电枪数量
 */
static THmiParamCmdResult ParamItemChargeGunCnt(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    {
        if (KfcSrtPhmSetU08(kSrtPhmChargeGunCnt, *data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU08(kSrtPhmChargeGunCnt, data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-充电枪类型
 */
static THmiParamCmdResult ParamItemChargeGunType(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    {
        if (KfcSrtPhmSetU08(kSrtPhmChargeGunType, *data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU08(kSrtPhmChargeGunType, data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}
/**
 * 参数表-Bms类型
 */
static THmiParamCmdResult ParamItemBmsType(Bool is_set, UInt8 *data, UInt32 len)
{
    if (is_set)
    {
        if (KfcSrtPhmSetU08(kSrtPhmBmsType, *data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
    else
    {
        if (KfcSrtPhmGetU08(kSrtPhmBmsType, data))
            return kHmiParamCmdResultOk;
        else
            return kHmiParamCmdResultDoErr;
    }
}

/**
 * 参数表汇总
 */
TParamItem g_param_tab0[15] = 
{
    {0, 20, ParamItemDevNum},                 ///< 设备编号
    {1,  2, ParamItemHardwareVersion},        ///< 硬件版本
    {2,  2, ParamItemBootSoftVersion},        ///< Boot软件版本
    {3,  2, ParamItemAppSoftVersion},         ///< App软件版本
    {4,  2, ParamItemParamTabVersion},        ///< 参数表版本
    {5,  4, ParamItemCan0Baud},               ///< can0波特率
    {6,  4, ParamItemCan1Baud},               ///< can1波特率
    {7,  4, ParamItemCan2Baud},               ///< can2波特率
    /// 预留
    {40,  1, ParamItemProjectType},            ///< 项目类型
    {41,  1, ParamItemModuleType},             ///< 模块类型
    {42,  1, ParamItemModuleCnt},              ///< 模块数量
    {43,  1, ParamItemContGroupType},          ///< 接触器群充类型
    {44,  1, ParamItemChargeGunCnt},           ///< 充电枪数量
    {45,  1, ParamItemChargeGunType},          ///< 充电枪类型
    {46,  1, ParamItemBmsType},                ///< Bms类型
};
