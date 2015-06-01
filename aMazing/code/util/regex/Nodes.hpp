#pragma once
#include"../../common/CommonDef.hpp"
#include<memory>
#include<vector>

namespace aMazing
{
	class Node
	{
	public:
		typedef size_t size_type;
		typedef std::vector<std::shared_ptr<Node> > NodeQueue;
		void setIdentifer(int id)
		{
			identifer = id;
		}
		int getIdentifer() const
		{
			return identifer;
		}
		void clearNeighbors()
		{
			neighbors.clear();
		}
		void addNeighbor(const std::shared_ptr<Node>& node)
		{
			neighbors.push_back(node);
		}
		const std::shared_ptr<Node>& operator [] (size_type index) const 
		{
			return neighbors[index];
		}
	private:
		//The identifer of the node.
		//!!!note that in one node set,only one node with specific identifer can exist.
		int identifer;
		//The neighbors that have bridge with current node
		NodeQueue neighbors;
	};
}
