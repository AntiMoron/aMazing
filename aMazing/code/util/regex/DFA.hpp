#pragma once
#include"FSM.hpp"
#include"../dynamic_matrix.hpp"
namespace aMazing
{
	class DFA :public FSM
	{
	public:
		void minimize()
		{
			DynamicMatrix<bool> distinct(nodes.size(),nodes.size());
			;
		}
	};
}
