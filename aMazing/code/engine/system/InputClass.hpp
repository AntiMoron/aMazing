#pragma once
#include<array>
namespace aMazing
{
	class InputClass
	{
	public:
		static InputClass& GetInst();

		std::array<bool, 256> keys;
		static InputClass* instance;
		unsigned short mouse_x, mouse_y;
	private:
		InputClass();
		~InputClass();
	};
}

#define INPUT (aMazing::InputClass::GetInst())