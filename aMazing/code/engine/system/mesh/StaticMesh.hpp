#pragma once
#include"../BasicObject.hpp"
#include"GPUVerticesBuffer.hpp"

namespace aMazing
{
	template<typename Type>
	using StaticMesh = GPUVerticesBuffer<Type>;
}
