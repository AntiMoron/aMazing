#pragma once

#include<functional>
#include<type_traits>

namespace common_tool
{

	template<typename T>
	struct func_type_wrapper
	{
		typedef T type;
	};

	template<typename T>
	using functionType = 
		typename func_type_wrapper<T>::type;
}
