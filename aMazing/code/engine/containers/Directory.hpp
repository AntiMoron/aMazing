#pragma once

#include"./String.hpp"
namespace aMazing
{
	//Minimize an directory string.
	extern aString minimizeDirectory(const aString& dir);

	//Transform '\\' into '/'
	extern aString transformSplashes(const aString& dir);
}
