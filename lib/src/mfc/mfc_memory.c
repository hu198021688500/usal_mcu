// =======================================
// Unit   : memory manager (内存管理单元)
// Version: 4.0.1.0 (build 2018.12.17)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#include "mfc_memory.h"
#include <stdlib.h>
#include <string.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 写 debug 相关函数 */

#if defined(_KYMemory_Debug_) && (defined(_WIN32) || defined(_WIN64))
   #include "KDClient.h"
   #define  InitDebug(AName)              KDInitDebug(AName)
   #define  FreeDebug()                   KDFreeDebug()
   #define  DebugOpened()                 KDebugOpened()
   #define  DebugWrite(AInfo)             KDebugWrite(AInfo)
   #if defined(_WIN64)
      #define DebugWriteFmt(AFormat, ...) KDebugWriteFmt(AFormat, __VA_ARGS__)
   #else
      #define DebugWriteFmt               KDebugWriteFmt
   #endif
   #ifndef _KYMemory_Detail_
      #define DetailWrite(AInfo)
      #define DetailWriteFmt(AFormat)
   #elif defined(_WIN64)
      #define DetailWrite(AInfo)          KDebugWrite(AInfo)
      #define DetailWriteFmt(AFormat,...) KDebugWriteFmt(AFormat, __VA_ARGS__)
   #else
      #define DetailWrite(AInfo)          KDebugWrite(AInfo)
      #define DetailWriteFmt              KDebugWriteFmt
   #endif
#else
   #define  InitDebug(AName)
   #define  FreeDebug()
   #define  DebugOpened()                 False
   #define  DebugWrite(AInfo)
   #define  DebugWriteFmt(AFormat, ...)
   #define  DetailWrite(AInfo)
   #define  DetailWriteFmt(AFormat, ...)
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef __cplusplus
extern "C"
{
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量及基础类型定义 */

typedef signed char     I8_t;
typedef unsigned char   Bool;
typedef unsigned char   Byte;
typedef unsigned short  Word;
typedef unsigned int    DWord;

// 大内存开关, 若打开则最大支持 1GB, 否则最大只能 1MB
#ifdef _KYMemory_BigMem_
   typedef DWord        TOffset;
   enum {_1K_MB = 20};
#else
   typedef Word         TOffset;
   enum {_1K_MB = 10};
#endif

#ifndef NULL
#define NULL   0
#endif

enum
{
   False = 0,
   True = 1,

   _1K_MM =(1<<_1K_MB)-1,
   _Idx_ML = _1K_MB+5,
   _Idx_MS = _Idx_ML/3+1,

   // 内存管理支持的最大块数, 取值范围: [1..16]
   _Max_BC = 4,
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 内存管理的相关函数 */

static const Byte _4b_H[16] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};
static const Byte _4b_L[16] = {0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1};

static Byte L_H_BB(Byte A)
{
   Byte _c7_ = ((A & 0xF0) != 0) ? 4 : 0;
   return _c7_ + _4b_H[A >> _c7_];
}

static Byte L_L_BB(Byte A)
{
   Byte _c7_ = ((A & 0x0F) == 0 && A != 0) ? 4 : 0;
   return _c7_ + _4b_L[(A >> _c7_) & 0x0F];
}

static Byte L_H_B20(DWord A)
{
   Byte _c7_ = 0;
   if ((A & 0xFF000) != 0)
      _c7_ = ((A & 0xF0000) != 0) ? 16 : 12;
   else if ((A & 0x0F00) != 0)
      _c7_ = 8;
   else if ((A & 0x00F0) != 0)
      _c7_ = 4;
   return _c7_ + _4b_H[A >> _c7_];
}

static Byte L_L_B20(DWord A)
{
   Byte _c7_ = 0;
   if ((A & 0x0FFF) == 0)
      _c7_ = ((A & 0xF000) != 0) ? 12 : 16;
   else if ((A & 0x000F) != 0)
      ;
   else if ((A & 0x00F0) != 0)
      _c7_ = 4;
   else
      _c7_ = 8;
   return _c7_ + _4b_L[(A >> _c7_) & 0x0F];
}

static int L_FL0(Byte* Aid, Byte Am, DWord Anm);
static int L_FL1(Byte* Aid, Byte Am, DWord Anm);
static int L_FL2(Byte* Aid, Byte Am, DWord Anm);
typedef int (*T_FL)(Byte* Aid, Byte Am, DWord Anm);

typedef struct
{
   DWord          ic;
   Byte*          ad;
   Byte*          id;
} _T_SI;

typedef struct
{
   Byte*          b;
   Byte*          e;
   DWord          mm;
   _T_SI          si[_1K_MB];
} _T_KB;

typedef struct
{
   TLockFunc      lk;
   TLockFunc      ul;
   TMalloc        _m;
   TFree          _f;
   Bool           inited;
   Bool           doing;
   Byte           bc;
   Byte           mb;
   int            ms;
   int            kc;
   DWord          uc;
   DWord          mm;
   _T_KB          kb[_Max_BC];
} _T_Mem;

static _T_Mem _ = {0};
static const T_FL _0_F[3] = {&L_FL0, &L_FL1, &L_FL2};

#define _L_lk() _.lk()
#define _L_ul() _.ul()

static void L_lk(void) {}
static void L_ul(void) {}

static Bool L_Ib(int n, _T_KB* Ai, TMemBlock* Ab)
{
   Bool _a7_ = False;
   DebugWriteFmt("  >>  L_Ib {No.: %d, Count: %d, Items: %p, Indexs: %p}",
                 n, Ab->Count, Ab->Items, Ab->Indexs);
   if ((Ab->Items != NULL) && (Ab->Indexs != NULL) && (Ab->Count > 0))
   {
      _T_SI* _32_;
      DWord _dd_ = Ab->Count <= _1K_MM ? Ab->Count : _1K_MM;
      DWord _27_ = _dd_;
      Byte* _5b_ = (Byte*)Ab->Items;
      Byte* _46_ = (Byte*)Ab->Indexs;
      Byte  _f8_ = L_H_B20(_dd_) - 1;
      Ai->b = _5b_;
      Ai->e = _5b_ + (_dd_ << 10);
      Ai->mm = 0;
      if (_.mb < _f8_ + 1)
      {
         _.mb = _f8_ + 1;
         _.ms = 1 << (10 + _f8_);
      }
      memset(_46_, 0, _dd_ << 4);
      memset(&Ai->si, 0, sizeof(Ai->si));
      for (; ;)
      {
         _32_ = Ai->si + _f8_;
         _32_->ic = 1 << (6 + _f8_);
         _32_->ad = _5b_;
         _32_->id = _46_;
         DebugWriteFmt("   .. L_Ib {SectNo: %d, ic: 0x%x, ad: %p, id: %p}",
                       _f8_, _32_->ic, _32_->ad, _32_->id);
         Ai->mm|= 1 << _f8_;
         _46_ += 1 << (4 + _f8_);
         _5b_ += 1 << (10 + _f8_);
         _27_ -= 1 << _f8_;
         if (_27_ != 0)
            _f8_ = L_H_B20(_27_) - 1;
         else
            break;
      }
      _.mm |= Ai->mm;
      _.kc += _dd_;
      _a7_ = True;
      DebugWriteFmt("   ++ L_Ib {No.: %d, Count: %d, mm: 0x%X, b: %p, e: %p}",
                    n, _dd_, Ai->mm, Ai->b, Ai->e);
   }
   DebugWriteFmt("  <<  L_Ib {return: %d, No.: %d, kc: %d, mm: 0x%X, mb: %d, ms: 0x%X}",
                 _a7_, n, _.kc, _.mm, _.mb, _.ms);
   return _a7_;
}

static Byte L_IB(TMemBlock* Akb, int Anm)
{
   _T_KB* _0_ = _.kb;
   _T_KB* _68_ = _0_ + (_Max_BC - 1);
   int _80_;
   _.bc = 0;
   _.kc = 0;
   _.mm = 0;
   _.mb = 0;
   _.ms = 0;
   DebugWriteFmt(" >>   L_IB {Akb: %p, Anm: %d}", Akb, Anm);
   for (_80_ = 0; _80_ < Anm; _80_++, Akb++)
      if (L_Ib(_80_, _0_, Akb) && ++_0_ > _68_)
         break;
   _.bc = _0_ - _.kb;
   _.uc = 0;
   _.inited = (_.bc != 0);
   DebugWriteFmt(" <<   L_IB {return: %d}", _.bc);
   return _.bc;
}

static DWord L_CC(Byte* Aid, Byte Av, Byte Am, DWord Ai, Bool Anf)
{
   DWord _75_ = 0;
   DWord _4a_ = Ai << (Am - Av);
   Byte* _af_ = Aid + (1 << (Am - 3));
   if (Av >= Am - 1)
   {
      _75_ = 1 << (Am - Av);
      if (Anf)
      {
         _4a_ += _75_ - 1;
         _af_ += _4a_ >> 3;
         *_af_ &= ~(0x80 >> (_4a_ & 7));
      }
   }
   else
   {
      DWord _51_ = _4a_ + (1 << (Am - Av));
      DWord _d9_ = _4a_ + ((_51_ - _4a_) >> 1);
      Byte* _41_ = _af_ + ((_d9_ + 7) >> 3);
      Byte* _59_ = _af_ + ((_51_ - 1) >> 3);
      Byte  _ed_ = _d9_ & 7;
      Byte  _a6_;
      if (_41_ > _59_ || (_41_ == _59_ && _ed_ == 0))
      {
         _a6_ = 8 - L_H_BB((Byte)(*_59_ << _ed_));
         _d9_ += _a6_;
         if (_d9_ >= _51_)
            _75_ = _51_ - _4a_;
         else
         {
            _75_ = _d9_ - _4a_ + 1;
            if (Anf)
               *_59_ &= ~(0x80 >> (_a6_ + _ed_));
         }
      }
      else
      {
         if (_ed_ != 0)
         {
            _a6_ = 8 - L_H_BB((Byte)(_41_[-1] << _ed_));
            if (_a6_ + _ed_ < 8)
            {
               _75_ = _d9_ - _4a_ + _a6_ + 1;
               if (Anf)
                  _41_[-1] &= ~(0x80 >> (_a6_ + _ed_));
            }
            else
               _d9_ += 8 - _ed_;
         }
         if (_75_ == 0)
         {
            for (_af_ = _41_; _41_ < _59_; _41_++)
               if (*_41_ != 0)
                  break;
            _a6_ = 8 - L_H_BB(*_41_);
            _d9_ += (_41_ - _af_) << 3;
            _d9_ += _a6_;
            if (_d9_ >= _51_)
               _75_ = _51_ - _4a_;
            else
            {
               _75_ = _d9_ - _4a_ + 1;
               if (Anf)
                  *_41_ &= ~(0x80 >> _a6_);
            }
         }
      }
   }
   DetailWriteFmt("  ..  CC {return: %u, Av: %d, Am: %d, Ai: %d, Anf: %d}",
                  _75_, Av, Am, Ai, Anf);
   return _75_;
}

static void L_SU(Byte* Aid, Byte Av, Byte Am, int Ai, DWord Anm)
{
   DWord _23_ = (Ai << (Am - Av)) + Anm - 1;
   Byte* _76_ = Aid + (1 << (Am - 3)) + (_23_ >> 3);
   *_76_ |= 0x80 >> (_23_ & 7);
   if (Av < 3)
      *Aid |= 0x80 >> ((1 << Av) + Ai);
   else if (Av != Am)
   {
      _76_ = Aid + (1 << (Av - 3)) + (Ai >> 3);
      *_76_ |= 0x80 >> (Ai & 7);
   }
   DebugWriteFmt("  ..  SU {Av: %d, Am: %d, Ai: %d, last: %d, Anm: %d}",
                 Av, Am, Ai, _23_, Anm);
}

static Bool L_IU(Byte* Aid, Byte Av, int  Ai, TOffset* Afs, Byte* Aud, DWord* Ap)
{
   Bool  _75_ = True;
   int   _e5_ = Av - 1;
   DWord _23_ = Ai >> 1;
   Byte* _41_;
   for (; _e5_ >= 3; _e5_--, _23_>>=1)
      if (Afs[_e5_] == _23_)
         break;
      else
      {
         Afs[_e5_] = _23_;
         _41_ = Aid + (1 << (_e5_ - 3)) + (_23_ >> 3);
         if (*_41_ & (0x80 >> (_23_ & 7)))
         {
            _75_ = False;
            *Aud = (Byte)_e5_;
            *Ap = _23_;
            break;
         }
      }
   if (_75_ && _e5_ < 3)
      do
      {
         if (Afs[_e5_] == _23_)
            break;
         Afs[_e5_] = _23_;
         if (*Aid & (0x80 >> ((1 << _e5_) + _23_)))
         {
            _75_ = False;
            *Aud = (Byte)_e5_;
            *Ap = _23_;
            break;
         }
         _23_ >>= 1;
      } while (--_e5_ >= 0);
   DetailWriteFmt("  ..  IU {return: %d, Av: %d, Ai: %d, ret-Aud: %d, ret-Ap: %d}",
                  _75_, Av, Ai, *Aud, *Ap);
   return _75_;
}

static Bool L_IE(Byte* Aid, Byte Av, Byte Am, DWord Ai, DWord Anm)
{
   Bool  _75_= True;
   Byte  _ed_ = Ai & 7;
   DWord _e1_= ((Anm - 1) >> (Am - Av)) + 1;
   DWord _51_ = Ai + _e1_;
   Byte* _41_ = Aid + (1 << (Av - 3));
   Byte* _59_ = _41_ + (_51_ >> 3);
   _41_ += (Ai + 7) >> 3;
   if (_41_ > _59_ || (_41_ == _59_ && _ed_ == 0))
   {
      _ed_ = *_59_ << _ed_;
      _75_ = ((_ed_ >> (8 - _e1_)) == 0);
   }
   else
   {
      if (_ed_ != 0)
      {
         _ed_ = _41_[-1] << _ed_;
         _75_ = (_ed_ == 0);
      }
      if (_75_)
      {
         _ed_ = _51_ & 7;
         if (_ed_ != 0 && (*_59_ >> (8 - _ed_)) != 0)
            _75_ = False;
         else
            for (; _41_ < _59_; _41_++)
               if (*_41_ != 0)
               {
                  _75_ = False;
                  break;
               }
      }
   }
   DetailWriteFmt("  ..  IE {return: %d, Av: %d, Am: %d, Ai: %d, Anm: %d}",
                  _75_, Av, Am, Ai, Anm);
   return _75_;
}

static Bool L_AE(Byte* Aid, Byte Av, Byte Am, DWord Ai, DWord Anm)
{
   Bool _75_ = True;
   for (; Av <= Am; Av++, Ai<<=1)
      if (!L_IE(Aid, Av, Am, Ai, Anm))
      {
         _75_ = False;
         break;
      }
   DetailWriteFmt("  ..  AE {return: %d, Am: %d, Anm: %d}", _75_, Am, Anm);
   return _75_;
}

static int L_FL0(Byte* Aid, Byte Am, DWord Anm)
{
   return Aid[0] < (1 << (3 - ((Anm-1) >> (Am-2))))
         && Aid[1] <= 7 && L_AE(Aid, 3, Am, 0, Anm) ? 0 : -1;
}

static int L_FL1(Byte* Aid, Byte Am, DWord Anm)
{
   int _75_ = -1;
   if (Aid[0] >= 0x40)
      ;
   else if ((Aid[0] & 0x13) == 0 && (Aid[1] & 0x0F) <= 1 && L_AE(Aid, 3, Am, 4, Anm))
      _75_ = 1;
   else if ((Aid[0] & 0x2C) == 0 && (Aid[1] & 0xF0) <= 0x10 && L_AE(Aid, 3, Am, 0, Anm))
      _75_ = 0;
   return _75_;
}

static int L_FL2(Byte* Aid, Byte Am, DWord Anm)
{
   int _75_ = -1;
   if (Aid[0] >= 0x40)
   {
      if (Aid[0] == 0x40 && (Aid[1] & 3) == 0 && L_AE(Aid, 4, Am, 12, Anm))
         _75_ = 3;
   }
   else if ((Aid[0] & 0x11) == 0 && (Aid[1] & 3) == 0 && L_AE(Aid, 4, Am, 12, Anm))
      _75_ = 3;
   else if ((Aid[0] & 0x12) == 0 && (Aid[1] & 0xC) == 0 && L_AE(Aid, 4, Am, 8, Anm))
      _75_ = 2;
   else if ((Aid[0] & 0x24) == 0 && (Aid[1] & 0x30) == 0 && L_AE(Aid, 4, Am, 4, Anm))
      _75_ = 1;
   else if ((Aid[0] & 0x28) == 0 && (Aid[1] & 0xC0) == 0 && L_AE(Aid, 4, Am, 0, Anm))
      _75_ = 0;
   return _75_;
}

static int L_FN(Byte* Aid, Byte Av, Byte Am, DWord Anm)
{
   int      _27_ = -1;
   int      _4f_ = (1 << Av) - 1;
   Byte*    _d_ = Aid + (1 << (Av - 3));
   Byte*    _f_ = _d_ + (1 << (Av - 3)) - 1;
   DWord    _3_ = 0;
   I8_t     _e7_ = -1;
   Byte     _d2_ = 1;
   Byte     _84_;
   DWord    _8c_;
   TOffset  _f4_[_Idx_ML];
   TOffset  _c_[_Idx_MS], _a0_[_Idx_MS];
   memset(_f4_, -1, sizeof(_f4_));
   do
   {
      if (*_f_ & _d2_)
      {
         _4f_--;
         _d2_ <<= 1;
      }
      else if ((DWord)_4f_ < _3_)
      {
         _4f_ = _c_[_e7_] - 1;
         _d2_ = 0x80 >> (_4f_ & 7);
         _f_ = _d_ + (_4f_ >> 3);
         if (--_e7_ >= 0)
         {
            _3_ = _a0_[_e7_] + 1;
            _8c_ = _c_[_e7_];
         }
         else
            _3_ = 0;
      }
      else if (!L_IU(Aid, Av, _4f_, _f4_, &_84_, &_8c_))
      {
         _3_ = L_CC(Aid, _84_, Am, _8c_, False);
         _8c_ <<= (Av - _84_);
         _3_ = _8c_ + ((_3_ - 1) >> (Am - Av)) + 1;
         _e7_++;
         _c_[_e7_] = _8c_;
         _a0_[_e7_] = _3_ - 1;
      }
      else if (L_AE(Aid, Av, Am, _4f_, Anm))
      {
         _27_ = _4f_;
         break;
      }
      else
      {
         _4f_--;
         _d2_ <<= 1;
      }
      if (_d2_ == 0)
      {
         _f_--;
         _d2_ = 1;
         while (_4f_ >= 0 && *_f_ == 0xFF)
         {
            _4f_ -= 8;
            _f_--;
         }
      }
   } while (_4f_ >= 0);
   return _27_;
}

static int L_FM(Byte* Aid, Byte Am)
{
   int      _27_ = -1;
   int      _4f_ = (1 << Am) - 1;
   Byte*    _d_ = Aid + (1 << (Am - 3));
   Byte*    _f_ = _d_ + (1 << (Am - 3)) - 1;
   DWord    _3_ = 0;
   I8_t     _e7_ = -1;
   Byte     _d2_ = 1;
   Byte     _84_;
   DWord    _8c_;
   TOffset  _f4_[_Idx_ML];
   TOffset  _c_[_Idx_MS], _a0_[_Idx_MS];
   memset(_f4_, -1, sizeof(_f4_));
   do
   {
      if (*_f_ & _d2_)
      {
         _4f_--;
         _d2_ <<= 1;
      }
      else if ((DWord)_4f_ < _3_)
      {
         _4f_ = _c_[_e7_] - 1;
         _d2_ = 0x80 >> (_4f_ & 7);
         _f_ = _d_ + (_4f_ >> 3);
         if (--_e7_ >= 0)
         {
            _3_ = _a0_[_e7_] + 1;
            _8c_ = _c_[_e7_];
         }
         else
            _3_ = 0;
      }
      else if (!L_IU(Aid, Am, _4f_, _f4_, &_84_, &_8c_))
      {
         _3_ = L_CC(Aid, _84_, Am, _8c_, False);
         _8c_ <<= (Am - _84_);
         _3_    += _8c_;
         _e7_++;
         _c_[_e7_] = _8c_;
         _a0_[_e7_] = _3_ - 1;
      }
      else
      {
         _27_ = _4f_;
         break;
      }
      if (_d2_ == 0)
      {
         _f_--;
         _d2_ = 1;
         while (_4f_ >= 0 && *_f_ == 0xFF)
         {
            _4f_ -= 8;
            _f_--;
         }
      }
   } while (_4f_ >= 0);
   return _27_;
}

static void* L_SN(_T_SI* Asi, Byte Ai, Byte Ab, DWord Anm)
{
   void* _75_ = NULL;
   DebugWriteFmt(">>    SN {Ai: %d, ic: %d, Anm: %d, Ab: %d}", Ai, Asi->ic, Anm, Ab);
   if (Asi->ic >= Anm)
   {
      Byte _3b_ = Ai + 6;
      Byte _52_ = _3b_ - Ab;
      int  _e5_;
      if (Asi->ic == (DWord)(1 << _3b_))
         _e5_ = (1 << _52_) - 1;
      else if (_52_ < 3)
         _e5_ = _0_F[_52_](Asi->id, _3b_, Anm);
      else if (_52_ == _3b_)
         _e5_ = L_FM(Asi->id, _3b_);
      else
         _e5_ = L_FN(Asi->id, _52_, _3b_, Anm);
      DebugWriteFmt("  ... SN {_e5_: %d}", _e5_);
      if (_e5_ >= 0)
      {
         L_SU(Asi->id, _52_, _3b_, _e5_, Anm);
         Asi->ic-= Anm;
         _.uc += Anm;
         _75_ = Asi->ad + (_e5_ << (Ab + 4));
      }
   }
   DebugWriteFmt("++    SN {return: %p, Ai: %d, ic: %d}", _75_, Ai, Asi->ic);
   return _75_;
}

static Bool L_SF(_T_SI* Asi, Byte Ai, Byte* Am)
{
   Bool _75_ = False;
   Byte _3b_ = Ai + 6;
   DebugWriteFmt("<<    SF {Ai: %d, ic: %d, ad: %p, Am: %p}", Ai, Asi->ic, Asi->ad, Am);
   if (Am >= Asi->ad)
   {
      DWord _d9_ = (Am - Asi->ad) >> 4;
      if (_d9_ < (DWord)(1 << _3b_))
      {
         Byte* _41_ = Asi->id;
         Byte  _3a_ = 0;
         Byte  _b2_= _3b_ + 1;
         if (_d9_ == 0)
            _b2_ = 0;
         else if ((_d9_ & 0xFFFFF) != 0)
            _b2_-= L_L_B20(_d9_);
         else
            _b2_-= L_L_BB((Byte)(_d9_ >> 20)) + 20;
         _d9_ >>= _3b_ - _b2_;
         if (_b2_ < 3)
            do
            {
               _3a_ = 0x80 >> ((1 << _b2_) + _d9_);
               if ((*_41_ & _3a_) != 0)
               {
                  _75_ = True;
                  break;
               }
               _d9_ <<= 1;
            } while (++_b2_ < 3);
         if (!_75_)
            for (; _b2_ <= _3b_; _b2_++, _d9_<<=1)
            {
               _41_ = Asi->id + (1 << (_b2_ - 3)) + (_d9_ >> 3);
               _3a_ = 0x80 >> (_d9_ & 7);
               if ((*_41_ & _3a_) != 0)
               {
                  _75_ = True;
                  break;
               }
            }
         if (_75_)
         {
            DWord _dd_ = 1;
            *_41_ &= ~_3a_;
            if (_b2_ != _3b_)
               _dd_ = L_CC(Asi->id, _b2_, _3b_, _d9_, True);
            Asi->ic += _dd_;
            _.uc -= _dd_;
         }
      }
   }
   DebugWriteFmt("~~    SF {return: %d, Ai: %d, ic: %d}", _75_, Ai, Asi->ic);
   return _75_;
}

static int L_BO(Byte* Am)
{
   int  _a7_ = -1;
   Byte _f8_;
   for (_f8_ = 0; _f8_ < _.bc; _f8_++)
      if (Am >= _.kb[_f8_].b && Am < _.kb[_f8_].e)
      {
         _a7_ = _f8_;
         break;
      }
   return _a7_;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 内存管理的相关函数 */

int KYMemory_Init(TLockFunc  lock,  TLockFunc unlock,
                  TMalloc    do_malloc, TFree do_free,
                  TMemBlock* blocks,      int count)
{
   int _a7_ = -3;
   if ((blocks == NULL) || (count <= 0)
                         || (do_malloc == &Malloc) || (do_free == &Free)
                         || ((lock != NULL) != (unlock != NULL))
                         || ((do_malloc != NULL) != (do_free != NULL)))
      _a7_ = -1;
   else if (_.inited)
      _a7_ = -2;
   else if (!_.doing)
   {
      _.doing = True;
      InitDebug("_KYMemory_1.1_");
      DebugWriteFmt(">>>>> KYMemory_Init {sizeof(TOffset): %d, lock: %p, unlock: %p,"
                    " _malloc: %p, _free: %p, blocks: %p, count: %d}",
                    sizeof(TOffset), lock, unlock, do_malloc, do_free, blocks, count);
      if (lock != NULL)
      {
         _.lk = lock;
         _.ul = unlock;
      }
      else
      {
         _.lk = &L_lk;
         _.ul = &L_ul;
      }
      _._m = do_malloc;
      _._f = do_free;
      _a7_ = L_IB(blocks, count);
      DebugWriteFmt("<<<<< KYMemory_Init {return: %d}", _a7_);
      _.doing = False;
   }
   return _a7_;
}

void KYMemory_Fini(void)
{
   if (_.inited && !_.doing)
   {
      _.doing = True;
      DebugWriteFmt("~~~~~ KYMemory_Fini {count: %d, kc: %d}", _.bc, _.kc);
      _.bc = 0;
      _.uc = 0;
      _.kc = 0;
      _.mm = 0;
      _.mb = 0;
      _.ms = 0;
      _._m = NULL;
      _._f = NULL;
      _.lk = &L_lk;
      _.ul = &L_ul;
      _.inited = False;
      FreeDebug();
      _.doing = False;
   }
}

int KYMemory_1KBCount(void)
{
   return _.inited ? _.kc : -1;
}

int KYMemory_CellCount(void)
{
   return _.inited ? (_.kc << 6) : -1;
}

int KYMemory_UsedCount(void)
{
   return _.inited ? (int)_.uc : -1;
}

int KYMemory_BlockCount(void)
{
   return _.inited ? _.bc : -1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 内存分配/释放函数 */

void* Malloc(int size)
{
   void* _a7_ = NULL;
   DebugWriteFmt(">>>   Malloc {size: %d, ms: %d, uc: %d}", size, _.ms, _.uc);
   if (size <= 0)
      ;
   else if (size <= _.ms)
   {
      DWord _dd_ = (size + 15) >> 4;
      Byte  _f8_ = L_H_B20((size - 1) >> 10);
      Byte  _ab_ = _f8_ != 0 ? _f8_ + 6 : L_H_BB((Byte)(_dd_ - 1));
      if (_f8_ < _.mb && _.inited)
      {
         _T_KB* _bb_;
         _T_KB* _8_ = _.kb + _.bc;
         DWord  _1a_ = (_1K_MM << _f8_) & _.mm;
         DWord  _64_;
         while (_1a_ != 0 && _a7_ == NULL)
         {
            _f8_ = L_L_B20(_1a_) - 1;
            _64_ = 1 << _f8_;
            _1a_ &= _1a_ - 1;
            for (_bb_ = _.kb; _bb_ < _8_ && _a7_ == NULL; _bb_++)
               if (_bb_->mm & _64_)
               {
                  _L_lk();
                  _a7_ = L_SN(_bb_->si + _f8_, _f8_, _ab_, _dd_);
                  _L_ul();
               }
         }
      }
      if (_a7_ == NULL && _._m != NULL)
         _a7_ = _._m(size);
   }
   else if (_._m != NULL)
      _a7_ = _._m(size);
   DebugWriteFmt("+++   Malloc {return: %p, uc: %d}", _a7_, _.uc);
   return _a7_;
}

void Free(void* mem)
{
   if (mem != NULL)
   {
      int _65_ = L_BO(mem);
      DebugWriteFmt("<<<   Free {memory: %p, block: %d}", mem, _65_);
      if (_65_ >= 0)
      {
         _T_KB* _bb_ = _.kb + _65_;
         DWord _66_ = (Byte*)mem - _bb_->b;
         DetailWriteFmt("   .. Free {_66_: 0x%X, mm: 0x%X}", _66_, _bb_->mm);
         if ((_66_ & 0x0F) == 0)
         {
            Byte _f8_ = L_H_B20((_66_ >> 10) ^ _bb_->mm) - 1;
            DetailWriteFmt("   .. Free {_f8_: %d}", _f8_);
            if (_.inited && (_bb_->mm & (1 << _f8_)) != 0)
            {
               _L_lk();
               L_SF(_bb_->si + _f8_, _f8_, mem);
               _L_ul();
            }
         }
      }
      else if (_._f != NULL)
         _._f(mem);
      DebugWriteFmt("~~~   Free {memory: %p, uc: %d}", mem, _.uc);
   }
}

#ifdef __cplusplus
}
#endif
