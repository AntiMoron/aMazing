#pragma once

#include <memory>
#include <string>
#include <vector>
#include <assimp/scene.h>

namespace aMazing
{
	class SceneAnimNode
	{
	public:
		SceneAnimNode() 
		{
			channelIndex = -1;
		}

		SceneAnimNode(const std::string& name)
		{
			this->name = name;
			channelIndex = -1;
		}

		SceneAnimNode(std::string&& name)
		{
			this->name = name;
			channelIndex = -1;
		}

		std::string name;
		//if the pointer managed is nullptr which means that this node doesn't have a parent node.
		std::shared_ptr<SceneAnimNode> parent;
		std::vector<std::shared_ptr<SceneAnimNode> > children;
		//most recently calculated transform in bind space
		aiMatrix4x4 localTransform;
		//most recently calculated transform in world space
		aiMatrix4x4 globalTransform;
		//index in the current animation's channel array. -1 if not animated.
		std::size_t channelIndex;
	};
}
