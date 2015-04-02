#pragma once

/**********Language****************************/
#define aNOEXCEPT  throw()
#define aCONSTEXPR 
/***********Macro Functions*******************/
#define aSAFE_RELEASE(x) do{if(!!(x)){(x)->Release(); (x) = nullptr;}}while(0)
#define aRETURN_ON_FAIL(x) if(FAILED(x)){ return E_FAIL;}
#define aOffsetof(s,x) ((std::size_t)&reinterpret_cast<const volatile char&>(((s*)nullptr)->x))
#define aRADIAN_TO_ANGLE(x) (x * 180.0 / (aPI))
#define aANGLE_TO_RADIAN(x) (x * aPI / 180.0)

/***********Common Values**************/
#define aPI 3.141592653589793


/**********Debugs************/
#ifdef OPEN_DEBUG_OPTION
#define aDBG(x)  do{std::cout << x;}while(0)
#else
#define aDBG(x) 
#endif
