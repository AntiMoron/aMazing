#pragma once
#include<type_traits>
#include<vector>
#include<memory>
#include"../../../util/any.hpp"
#include"../../../common/CommonDef.hpp"

namespace aMazing
{
	namespace json
	{
		enum aJsonType
		{
			aJsonBoolean,
			aJsonNumber,
			aJsonString,
			aJsonObject,
			aJsonArray,

			aJsonVirtualRoot,
			aJsonUnknown,
		};
		class aJsonNode
		{
		private:
			/*
			This should be disabled
			*/
			template<typename T, aJsonType V = aJsonUnknown>
			virtual T getValue()
			{
				std::enable_if<V == aJsonUnknown, T>::type result;
				return result;
			}

			template<>
			std::vector<std::shared_ptr<aJsonNode> > getValue<std::vector<std::shared_ptr<aJsonNode> >, aJsonArray>()
			{
				return data.reach<std::vector<std::shared_ptr<aJsonNode> > >();
			}

			template<>
			bool getValue<bool, aJsonBoolean>()
			{
				return data.reach<bool>();
			}

			template<>
			std::string getValue<std::string, aJsonString>()
			{
				return data.reach<std::string>();
			}

			template<>
			double getValue<double, aJsonNumber>()
			{
				return data.reach<double>();
			}

			template<>
			aJsonNode getValue<aJsonNode, aJsonObject>()
			{
				return data.reach<aJsonNode>();
			}
		public:
			/*
			@brief : to set the parent of this node.
			*/
			void setParent(const std::shared_ptr<aJsonNode>& pParent) aNOEXCEPT
			{
				parent = pParent;
			}
			/*
			@brief : to insert the child into the node.
			@param pChild the pointer of child you want to insert.
			*/
			void addChild(const std::shared_ptr<aJsonNode>& pChild)
			{
				children.emplace_back(pChild);
			}
			/*
			@brief : to insert the child into the node.
			@param index : int   the index of element you want to insert to. -1 insert to the back.
			@param pChild the pointer of child you want to insert.
			*/
			void addChild(int index, const std::shared_ptr<aJsonNode>& pChild)
			{
				children.insert(children.begin() + index, pChild);
			}
			/*
			@brief : delete a child.
			@param : index. indicates the child's index in the array.
			*/
			bool delChild(int index) aNOEXCEPT
			{
				if (aSTL_OUT_OF_RANGE(index, children))
				{
					return false;
				}
				children.erase(children.begin() + index);
				return true;
			}

			void setValue(const any& tData)
			{
				data = tData;
			}

			void setJsonType(aJsonType type)
			{
				jsonType = type;
			}

			aJsonType getJsonType() const aNOEXCEPT
			{
				return jsonType;
			}
		private:
			std::vector<std::shared_ptr<aJsonNode> > children;
			std::shared_ptr<aJsonNode> parent;
			aJsonType jsonType;
			any data;
		};
	}
}
