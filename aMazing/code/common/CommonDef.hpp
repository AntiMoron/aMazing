#pragma once

#include<memory>

#define aNOEXCEPT  throw()
#define aSAFE_RELEASE(x) do{if(!!(x)){(x)->Release(); (x) = nullptr;}}while(0)
#define aRETURN_ON_FAIL(x) if(FAILED(x)){ return E_FAIL;}
#define aOffsetof(s,x) ((std::size_t)&reinterpret_cast<const volatile char&>(((s*)nullptr)->x))
