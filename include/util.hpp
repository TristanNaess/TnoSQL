#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include "document.hpp"

namespace util
{
    std::unordered_map<std::string, Data> build_structure(const std::string& data);
}

#endif//__UTIL_HPP__
