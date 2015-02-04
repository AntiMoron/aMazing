#pragma once

#define ALIGN16 __declspec(align(16))

#define SAFE_CREATE(varPtr,varType) do{varPtr = nullptr;\
	varPtr = new varType; }while (0)
#define SAFE_DELETE(varPtr) do{if(varPtr){delete varPtr;varPtr = nullptr;}}while(0)

#define SAFE_CREATE_MULTI(varPtr,varType,count) do{varPtr = nullptr;\
	varPtr = new varType[count]; \
	}while (0)

#define SAFE_DELETE_MULTI(varPtr) do{if(varPtr){delete [] varPtr;\
	varPtr = nullptr;\
	}\
}while (0)


#define SAFE_RELEASE(x) do{ if((x) != nullptr){(x)->Release(); (x) = nullptr;} \
} while (0)
