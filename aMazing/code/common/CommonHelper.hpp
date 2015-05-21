#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace aMazing
{
	namespace detail
	{
		struct yes_type
		{
			char f[1];
		};
		struct no_type
		{
			char f[2];
		};

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
}
