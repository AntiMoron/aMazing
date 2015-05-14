#pragma once
#include"../engine/Config.hpp"
#include<iostream>
#pragma warning(disable : 4127)
//Configures
#if  defined(_MSC_VER)
#define VC_COMPILER
#endif

//Debug
#ifdef OPEN_DEBUG_OPTION
#define aDBG(x)  do{std::cout << x << std::endl;}while(0)
#else
#define aDBG(x) 
#endif
#define aTHROW_ON(expr,exception,msg) do{throw exception(msg);}while(0)

//Language
#define aNOEXCEPT  throw()
#define aCONSTEXPR 
#define aSTL_OUT_OF_RANGE(index,container)  (((index) >= (container).size())||((index) < 0))

//Macro functions
#ifdef VC_COMPILER
#define MULTI_LINE_MACRO_BEGIN do {  
#define MULTI_LINE_MACRO_END \
} while (false)
#else
#define MULTI_LINE_MACRO_BEGIN do {  
#define MULTI_LINE_MACRO_END } while (bool(false))
#endif

#define aSAFE_RELEASE(x) \
MULTI_LINE_MACRO_BEGIN\
	if (!!(x)){ (x)->Release(); \
	aDBG("address at[" << (x) <<"] released.");\
	(x) = nullptr;}\
MULTI_LINE_MACRO_END
#define aRETURN_ON_FAIL(x) if(FAILED(x)){ return E_FAIL;}
#define aOffsetof(s,x) ((std::size_t)&reinterpret_cast<const volatile char&>(((s*)nullptr)->x))
#define aRADIAN_TO_ANGLE(x) (x * 180.0 / (aPI))
#define aANGLE_TO_RADIAN(x) (x * aPI / 180.0)

//Common literals
#define aPI 3.141592653589793
