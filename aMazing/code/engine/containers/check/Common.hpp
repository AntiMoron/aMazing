#pragma once
#include"../../../common/CommonHelper.hpp"
#include<type_traits>

namespace aMazing
{
	namespace detail
	{
		template<typename T>
		class aHasAllocatorType
		{
		private:
			template<typename C>
				const static yes_type& test(typename C::allocator_type*);
			template<typename C>
				const static no_type& test(...);
		public:
			const static bool value = (sizeof(test<T>(nullptr)) == sizeof(yes_type));
		};
		
		template<typename Class, typename ReturnType,
			typename ...Arguments>
		class aHasGetAllocatorStaticFunction
		{
		private:
			typedef typename std::remove_const<ReturnType>::type non_const_return_type;
			typedef typename std::remove_reference<non_const_return_type>::type rawReturnType;
			template<typename C, typename R, typename ...Args>
			static
				typename std::conditional<
				std::is_same<decltype(C::get_allocator(std::declval<Args>()...)), rawReturnType>::value ||
				std::is_same<decltype(C::get_allocator(std::declval<Args>()...)), rawReturnType&>::value ||
				std::is_same<decltype(C::get_allocator(std::declval<Args>()...)), const rawReturnType>::value ||
				std::is_same<decltype(C::get_allocator(std::declval<Args>()...)), const rawReturnType&>::value,
				yes_type,
				no_type>::type& test(C*);
			template<typename C, typename R, typename ...Args>
			static no_type& test(...);
		public:
			const static bool value = (sizeof(test<Class, ReturnType, Arguments...>(nullptr)) == sizeof(yes_type));
		};

		template<typename Class, typename ReturnType,
			typename ...Arguments>
		class aHasGetAllocatorMemberFunction
		{
		private:
			typedef typename std::remove_const<ReturnType>::type non_const_return_type;
			typedef typename std::remove_reference<non_const_return_type>::type rawReturnType;
			template<typename C, typename R, typename ...Args>
			static
				typename
				std::conditional<
				std::is_same<decltype(std::declval<C>().get_allocator(std::declval<Args>()...)), rawReturnType>::value ||
				std::is_same<decltype(std::declval<C>().get_allocator(std::declval<Args>()...)), rawReturnType&>::value ||
				std::is_same<decltype(std::declval<C>().get_allocator(std::declval<Args>()...)), const rawReturnType>::value ||
				std::is_same<decltype(std::declval<C>().get_allocator(std::declval<Args>()...)), const rawReturnType&>::value,
				yes_type,
				no_type>::type& test(C*);
			template<typename C, typename R, typename ...Args>
			static no_type& test(...);
		public:
			const static bool value = (sizeof(test<Class, ReturnType, Arguments...>(nullptr)) == sizeof(yes_type));
		};
	}

	template<typename T>
	class aIsMovable
	{
	private:
		template<typename C>
		static detail::yes_type& test(C*);
		template<typename C>
		static detail::no_type& test(...);
	public:
		const static bool value = ;
	};
}
