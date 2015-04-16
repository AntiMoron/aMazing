#include"aThreadSafeSingleton.hpp"
using namespace aMazing;

template<typename T>
T aThreadSafeSingleton<T>::instance = nullptr;