#pragma once
#include"Common.hpp"
namespace aMazing
{
	//Container is the type of the container.
	//Allocator is the type of allocator used.
	//Type is the type of elements that container contains.
	template<typename Container,typename Allocator,typename Type>
	class aIsInsertable
	{
	private:
		template<typename C, typename A, typename T>
		static typename std::conditional<
			detail::aHasAllocatorType<Container>::value &&
			detail::aHasGetAllocatorStaticFunction<Container,Allocator>::value &&
			detail::aHasGetAllocatorMemberFunction<Container,Allocator>::value,
			detail::yes_type,
			detail::no_type>::type& test(C*);
		template<typename C,typename A,typename T>
		static detail::no_type& test(...);
	public:
		const static value = (sizeof(test<Container, Allocator, Type>(nullptr)) == sizeof(detail::yes_type));
	};
}
