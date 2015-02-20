#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace aMazing
{
	class aiStringLess
	{
	public:
		bool operator()(const aiString& aiA, const aiString& aiB)const
		{
			std::string a = aiA.C_Str();
			std::string b = aiB.C_Str();
			return a < b;
		}
	};
}
