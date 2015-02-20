#pragma once
#include"../common/CommonTemplates.hpp"
#include"../engine/exception/MemoryException.hpp"
#include<type_traits>
#include<typeinfo>

namespace aMazing
{
	class any
	{
	public:
		any()
		{
			content = nullptr;
		}

		template<typename type>
		any(const type& value)
		{
			content = new holder<typename decay<const type>::type>(value);
		}

		any(const any& other)
		{
			if (other.empty())
			{
				content = nullptr;
			}
			else
			{
				content = other.content->clone();
			}
		}

		any(any&& other)
		{
			content = other.content;
			other.content = nullptr;
		}

		template<typename type>
		any(type&& value,
			typename std::enable_if<!std::is_same<any&, type>::value>::type* = 0,
			typename std::enable_if<!std::is_const<type>::value>::type* = 0)
		{
			content = new holder<typename decay<type>::type>(static_cast<type&&>(value));
		}

		any& swap(any& rhs) _NOEXCEPT
		{
			auto t = content;
			content = rhs.content;
			rhs.content = t;
			return *this;
		}

		any& operator = (const any& rhs)
		{
			any(rhs).swap(*this);
			return *this;
		}

		any& operator = (any&& rhs) _NOEXCEPT
		{
			rhs.swap(*this);
			any().swap(rhs);
			return *this;
		}

		template<typename type>
		any& operator = (type&& rhs)
		{
			any(static_cast<type&&>(rhs)).swap(*this);
			return *this;
		}

		~any()
		{
			if (content)
			{
				delete content;
				content = nullptr;
			}
		}


		/*
		*@brief:
		*To reach the data saved in 'any' object
		*read & write operation is open.
		*/
		template<typename type>
		type& reach() throw(MemoryException)
		{
			if (sizeof(type) > content->getDataSize())
			{
				throw MemoryException("Memory access out of range.");
			}
			return *reinterpret_cast<type*>(content->reach());
		}
		/*
		*@brief:
		*To reach the data saved in 'any' object.
		*This function is **Not recommanded** because this function is unsafe.
		*read & write operation is open.
		*More effective.
		*/
		template<typename type>
		type& unsafeReach() _NOEXCEPT
		{
			return *reinterpret_cast<type*>(content->reach());
		}

		const bool empty() const
		{
			return !content;
		}

		const std::type_info& type() const _NOEXCEPT
		{
			return content ? content->type() : typeid(void);
		}
	private:
		class placeHolder
		{
		public:
			virtual placeHolder* clone() const = 0;
			virtual void* reach() = 0;
			virtual ~placeHolder(){}
			virtual const std::type_info & type() const _NOEXCEPT = 0;
			virtual size_t getDataSize() const = 0;
		};

		template<typename T>
		class holder : public placeHolder
		{
		public:
			holder(const T& content)
			{
				this->content = content;
			}

			holder(T&& content)
			{
				this->content = content;
			}

			virtual const std::type_info& type() const _NOEXCEPT
			{
				return typeid(T);
			}

			void* reach()
			{
				return aAddressof(content);
			}

			virtual size_t getDataSize() const
			{
				return sizeof(T);
			}

			virtual placeHolder* clone() const
			{
				return new holder(content);
			}
		private:
			T content;
		};

		placeHolder* content;
	};
}