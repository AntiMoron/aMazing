#include"./Memory.hpp"
using namespace aMazing;

template<typename Alloc>
std::unordered_map<void*,size_t> aMemory::blockElemCnt;