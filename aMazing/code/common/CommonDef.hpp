#pragma once

#include<memory>


#define SAFE_RELEASE(x) do{if(!!(x)){(x)->Release(); (x) = nullptr;}}while(0)
#define RETURN_ON_FAIL(x) if(FAILED(x)){ return E_FAIL;}
#define aOffsetof(s,x) ((std::size_t)&reinterpret_cast<const volatile char&>(((s*)nullptr)->x))
