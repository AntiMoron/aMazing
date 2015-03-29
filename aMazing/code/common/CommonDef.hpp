#pragma once

#include<memory>

#define aPI 3.141592653589793
#define aNOEXCEPT  throw()
#define aSAFE_RELEASE(x) do{if(!!(x)){(x)->Release(); (x) = nullptr;}}while(0)
#define aRETURN_ON_FAIL(x) if(FAILED(x)){ return E_FAIL;}
#define aOffsetof(s,x) ((std::size_t)&reinterpret_cast<const volatile char&>(((s*)nullptr)->x))
#define aRADIAN_TO_ANGLE(x) (x * 180.0 / (aPI))
#define aANGLE_TO_RADIAN(x) (x * aPI / 180.0)