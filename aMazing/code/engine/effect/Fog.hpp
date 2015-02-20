#pragma once
#include<array>
#include"CommonDxSupport.hpp"
#include"EffectClass.hpp"

namespace aMazing
{
	class Fog :public EffectClass
	{
	public:
		Fog(){}
		~Fog(){}
		HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
		{
			HRESULT hr = E_FAIL;
			hr = EffectClass::Initialize(device, context);
			if (FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}

		void Render(ID3D11Device* device, ID3D11DeviceContext* context)
		{
			;
		}

		std::array<float, 4> getFogColor()const
		{
			return std::array<float, 4>{{ color[0], color[1], color[2], color[3] }};
		}
		void setFogColor(std::array<float, 4> color)
		{
			color[0] = color[0];
			color[1] = color[1];
			color[2] = color[2];
			color[3] = color[3];
		}

		void setFogNear(float fogNear)
		{
			this->fogNear = fogNear;
		}
		float getFogNear() const
		{
			return fogNear;
		}
		void setFogFar(float fogFar)
		{
			this->fogFar = fogFar;
		}
		float getFogFar()const
		{
			return fogFar;
		}
	private:
		//the near distance of fog
		float fogNear;
		//the far distance of fog
		float fogFar;
		//the color of fog
		float color[4];
	};
}
