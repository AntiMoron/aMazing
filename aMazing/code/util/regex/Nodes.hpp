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
	private:
		//The identifer of the node.
		//!!!note that in one node set,only one node with specific identifer can exist.
		int identifer;
	};
}
