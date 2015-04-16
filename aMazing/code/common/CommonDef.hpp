#pragma once
#include<iostream>
/**********Debugs************/

#ifdef OPEN_DEBUG_OPTION
#define aDBG(x)  do{std::cout << x << std::endl;}while(0)
#else
#define aDBG(x) 
#endif
#define aTHROW_ON(expr,exception,msg) do{throw exception(msg);}while(0)

/**********Language****************************/

#define aNOEXCEPT  throw()
#define aCONSTEXPR 
#define aSTL_OUT_OF_RANGE(index,container)  (((index) >= (container).size())||((index) < 0))

/***********Macro Functions*******************/

#define aSAFE_RELEASE(x) do{if(!!(x)){(x)->Release(); (x) = nullptr; aDBG("address at[" << (x) <<"] released."); }}while(0)
#define aRETURN_ON_FAIL(x) if(FAILED(x)){ return E_FAIL;}
#define aOffsetof(s,x) ((std::size_t)&reinterpret_cast<const volatile char&>(((s*)nullptr)->x))
#define aRADIAN_TO_ANGLE(x) (x * 180.0 / (aPI))
#define aANGLE_TO_RADIAN(x) (x * aPI / 180.0)

/***********Common Macros**************/

#define aPI 3.141592653589793
