#pragma once
#include"ShaderManager.h"
#include"FrameBuffer.h"
#include<functional>


class EffectClass
{
public:
	EffectClass();
	~EffectClass();

	HRESULT Initialize(ID3D11Device* device,
		ID3D11DeviceContext* context);
	HRESULT Shutdown();

	void setRenderTarget(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11DepthStencilView* depth);

	void bindVS(ID3D11Device* device,
		ID3D11DeviceContext* context,
		unsigned int textureSlot);

	void bindPS(ID3D11Device* device,
		ID3D11DeviceContext* context,
		unsigned int textureSlot);

	bool isInited()const;

	//void Render
	//Call the renderFunction function in argument-list to render Objects.
protected:
	FrameBuffer fbo;
	bool is_init;
private:

	template<typename T>
	struct func_type_wrapper
	{
		typedef T type;
	};
};

