#pragma once
#include<unordered_map>
#include<unordered_set>
#include"Nodes.hpp"

namespace aMazing
{
//	template<typename AlphaType>
	typedef char AlphaType;
	class FSM
	{
	public:
		struct Bridge
		{
			int start;
			int end;
		};
		int getStartState() const
		{
			return startState;
		}
		void setStartState(int id)
		{
			startState = id;
		}
		void addNode(int id)
		{
			auto node = std::make_shared<Node>();
			node->setIdentifer(id);
			nodes.push_back(node);
		}
		bool eraseNode(int id)
		{
			int index = searchNode(id);
			if (index == -1)
			{
				return false;
			}
			nodes.erase(nodes.begin() + index);
			return true;
		}
		void clear()
		{
			nodes.clear();
			endStates.clear();
			transitions.clear();
		}
		bool bindA2B(int id_a,int id_b,AlphaType ch)
		{
			int index_a = searchNode(id_a);
			int index_b = searchNode(id_b);
			if (index_a == -1 ||
				index_b == -1)
			{
				return false;
			}
			transitions[ch].push_back({id_a,id_b});
		}
		const std::unordered_set<AlphaType>& getAlphabet(int index) const
		{
			return alphabet;
		}
		bool addEndState(int id)
		{
			int index = searchNode(id);
			if (index == -1)
			{
				return false;
			}
			endStates.insert(id);
			return true;
		}
		const std::unordered_set<int>& getEndStates()
		{
			return endStates;
		}
	protected:
		int searchNode(int id)
		{
			for (size_t i = 0; i < nodes.size(); ++i)
			{
				if (nodes[i]->getIdentifer() == id)
					return i;
			}
			return -1;
		}

		int startState;
		std::unordered_set<int> endStates;
		std::unordered_set<AlphaType> alphabet;
		std::unordered_map<AlphaType,std::vector<Bridge> > transitions;
		Node::NodeQueue nodes;
	};
}
