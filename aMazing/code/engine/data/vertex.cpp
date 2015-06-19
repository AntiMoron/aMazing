#include"vertex.hpp"
using namespace aMazing;

aMazing::VertexType aMazing::detail::getVertexTypeByLayoutName(const std::string& layoutName)
throw(FailureException)
{
	if (vertexTypeMapper.find(layoutName) != vertexTypeMapper.end())
	{
		return vertexTypeMapper[layoutName];
	}
	throw FailureException("Config Error:Invalid Layout Name");
}


const D3D11_INPUT_ELEMENT_DESC Vertex::input_layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, aOffsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, aOffsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, aOffsetof(Vertex, texture), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const D3D11_INPUT_ELEMENT_DESC SkinVertex::input_layout[] =
{
	{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, aOffsetof(SkinVertex, boneIndices), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, aOffsetof(SkinVertex, weights), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, aOffsetof(SkinVertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, aOffsetof(SkinVertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, aOffsetof(SkinVertex, texture), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
