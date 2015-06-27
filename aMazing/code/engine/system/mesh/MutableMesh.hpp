#pragma once
#include"BaseMesh.hpp"
#include"GPUMutableVerticesBuffer.hpp"

namespace aMazing
{
	template<typename Type>
	using MutableMesh = GPUMutableVerticesBuffer<Type>;
}
