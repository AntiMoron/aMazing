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
			char alpha;
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
			//check start & end states.
			if (startState == id)
			{
				return false;
			}
			auto posInEndStates = endStates.find(id);
			if (posInEndStates == endStates.end())
			{
				endStates.erase(posInEndStates);
			}
			//check nodes
			int index = searchNode(id);
			if (index == -1)
			{
				return false;
			}
			nodes.erase(nodes.begin() + index);
			//check transitions.
			for (AlphaType a : alphabet)
			{
				auto& trans = transitions[a];
				for (auto it = trans.begin(); it != trans.end(); )
				{
					if (it->start == id || 
						it->end == id)
					{
						it = trans.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
			return true;
		}
		void clear()
		{
			nodes.clear();
			endStates.clear();
			transitions.clear();
		}
		bool bindA2B(int id_a, int id_b, AlphaType ch)
		{
			int index_a = searchNode(id_a);
			int index_b = searchNode(id_b);
			if (index_a == -1 ||
				index_b == -1)
			{
				return false;
			}
			transitions[ch].push_back({ id_a, id_b, ch });
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
		inline bool isFinal(int id)
		{
			return endStates.find(id) != endStates.end();
		}
		//return 
		int lookUpTransitions(int id_s, AlphaType a)
		{
			auto& trans = transitions[a];
			for (int i = 0; i < trans.size(); i++)
			{
				if (trans[i].start == id_s)
				{
					return trans[i].end;
				}
			}
			throw std::exception("No transition result found.");
		}
		void display()
		{
			aDBG("Alphabet : ");
			for (AlphaType a : alphabet)
			{
				aDBG_OLD_LINE(a);
			}
			aDBG("\nNodes : ");
			for (const auto& node : nodes)
			{
				aDBG_OLD_LINE(node->getIdentifer());
			}
			aDBG("\nTransitions : ");
			for (AlphaType a : alphabet)
			{
				for (const auto& trans : transitions[a])
				{
					aDBG(trans.start << "-" << a << "->" << trans.end);
				}
			}
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
		void getNodeTransitions(int id, std::vector<Bridge>& asStart, std::vector<Bridge>& asEnd)
		{
			for (AlphaType a : alphabet)
			{
				auto& trans = transitions[a];
				for (int i = 0; i < trans.size(); ++i)
				{
					if (trans[i].start == id)
					{
						asStart.push_back(trans[i]);
					}
					if (trans[i].end == id)
					{
						asEnd.push_back(trans[i]);
					}
				}
			}
		}
		int startState;
		std::unordered_set<int> endStates;
		std::unordered_set<AlphaType> alphabet;
		std::unordered_map<AlphaType,std::vector<Bridge> > transitions;
		Node::NodeQueue nodes;
	};
}
