#include "kfc_srt.h"
#include "srt.h"

///-----------------------plm掉电不存储接口------------------------///

Bool KfcSrtPlmSetBit(UInt32 addr, UInt32 bit, Bool value)
{
    return TSrtPlmSetBit(&g_srt, addr, bit, value);
}

Bool KfcSrtPlmSetU08(UInt32 addr, UInt8 value)
{
    return TSrtPlmSetU08(&g_srt, addr, value);
}

Bool KfcSrtPlmSetU16(UInt32 addr, UInt16 value)
{
    return TSrtPlmSetU16(&g_srt, addr, value);
}

Bool KfcSrtPlmSetU32(UInt32 addr, UInt32 value)
{
    return TSrtPlmSetU32(&g_srt, addr, value);
}

Bool KfcSrtPlmSetMem(UInt32 addr, UInt8 *data, UInt32 len)
{
    return TSrtPlmSetMem(&g_srt, addr, data, len);
}


Bool KfcSrtPlmGetBit(UInt32 addr, UInt32 bit, Bool* value)
{
    return TSrtPlmGetBit(&g_srt, addr, bit, value);
}

Bool KfcSrtPlmGetU08(UInt32 addr, UInt8* value)
{
    return TSrtPlmGetU08(&g_srt, addr, value);
}

Bool KfcSrtPlmGetU16(UInt32 addr, UInt16* value)
{
    return TSrtPlmGetU16(&g_srt, addr, value);
}

Bool KfcSrtPlmGetU32(UInt32 addr, UInt32* value)
{
    return TSrtPlmGetU32(&g_srt, addr, value);
}

UInt32 KfcSrtPlmGetMem(UInt32 addr, UInt8 *buffer, UInt32 size)
{
    return TSrtPlmGetMem(&g_srt, addr, buffer, size);
}



///-----------------------phm掉电存储接口------------------------///

Bool KfcSrtPhmSetBit(UInt32 addr, UInt32 bit, Bool value)
{
    return TSrtPhmSetBit(&g_srt, addr, bit, value);
}

Bool KfcSrtPhmSetU08(UInt32 addr, UInt8 value)
{
    return TSrtPhmSetU08(&g_srt, addr, value);
}

Bool KfcSrtPhmSetU16(UInt32 addr, UInt16 value)
{
    return TSrtPhmSetU16(&g_srt, addr, value);
}
Bool KfcSrtPhmSetU32(UInt32 addr, UInt32 value)
{
    return TSrtPhmSetU32(&g_srt, addr, value);
}
Bool KfcSrtPhmSetMem(UInt32 addr, UInt8 *data, UInt32 len)
{
    return TSrtPhmSetMem(&g_srt, addr, data, len);
}


Bool KfcSrtPhmGetBit(UInt32 addr, UInt32 bit, Bool* value)
{
    return TSrtPhmGetBit(&g_srt, addr, bit, value);
}

Bool KfcSrtPhmGetU08(UInt32 addr, UInt8* value)
{
    return TSrtPhmGetU08(&g_srt, addr, value);
}

Bool KfcSrtPhmGetU16(UInt32 addr, UInt16* value)
{
    return TSrtPhmGetU16(&g_srt, addr, value);
}

Bool KfcSrtPhmGetU32(UInt32 addr, UInt32* value)
{
    return TSrtPhmGetU32(&g_srt, addr, value);
}

UInt32 KfcSrtPhmGetMem(UInt32 addr, UInt8 *buffer, UInt32 size)
{
    return TSrtPhmGetMem(&g_srt, addr, buffer, size);
}


