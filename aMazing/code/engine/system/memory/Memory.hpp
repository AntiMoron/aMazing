#pragma once
#include"../../../common/CommonDef.hpp"
#include"../../exception/MemoryException.hpp"
#include<memory>
#include<unordered_map>

namespace aMazing
{
	/*
	!!! Not thread-safe
	*/
	template<typename Alloc>
	class aMemory
	{
	public:
		//Type for the type of the elements
		//Args for the type of parameter of constructor 
		template<typename Type,typename ...Args>
		static Type* createMemory (Alloc& actor,size_t count,Args... args) aNOEXCEPT
		{
			Type* ret = std::allocator_traits<Alloc>::allocate(actor, count);
			if (std::is_constructible<Type>::value)
			{
				for (size_t cur = 0; cur < count; cur++)
				{
					std::allocator_traits<Alloc>::construct(actor, ret, args...);
				}
			}
			blockElemCnt[reinterpret_cast<void*>(ret)] = sizeof(Type) * count;
			return ret;
		}

		template<typename Type>
		static void deleteMemory(Alloc& actor,Type* p) throw(MemoryException)
		{
			void* vp = reinterpret_cast<void*>(p);
			if (blockElemCnt.find(vp) != blockElemCnt.end())
			{
				/*static_assert(blockElemCnt[vp] % sizeof(Type) != 0,
					"Incorrect element type.");*/
				size_t elemCnt = blockElemCnt[vp] / sizeof(Type);
				if (std::is_destructible<Type>::value)
				{
					for (size_t cur = 0; cur < elemCnt; cur++)
					{
						std::allocator_traits<Alloc>::destroy(actor, p + cur);
					}
				}
				std::allocator_traits<Alloc>::deallocate(actor, p, elemCnt);
			}
		}
	private:
		//saving the bytes of the memory that pointers point.
		static std::unordered_map<void*,size_t> blockElemCnt;
	};

	template<typename Alloc>
	std::unordered_map<void*, size_t> aMemory<Alloc>::blockElemCnt;
}
