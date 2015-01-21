#pragma once
#include"D3DClass.h"
#include<memory>
#include"CommonUtil.hpp"

class BasicPrimitive
{
public:
	BasicPrimitive(){}
	virtual ~BasicPrimitive(){}
	HRESULT Initialize(D3DClass& d3dObj)
	{
		d3dPtr.reset(aAddressof(d3dObj));
		return S_OK;
	}
protected:
	std::shared_ptr<D3DClass> d3dPtr;
};