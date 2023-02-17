#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include "document.hpp"
#include <optional>

namespace util
{
    // construct in memory structure
    std::unordered_map<std::string, Data> build_structure(const std::string& data);

    // all extract_*_*() rely on verified strings

    // extract types from object or array string
    std::string_view extract_object_string(std::string_view line);
    std::string_view extract_array_string(std::string_view line);
    
    // pull field from object string matching key
    std::string_view extract_object_field(std::string_view line, std::string_view key);

    // pull nth element from array string
    std::string_view extract_array_field(std::string_view line, size_t index);

    // verify validity of type strings; return none option on success or string on error
    std::optional<std::string> verify_object(std::string_view line);
    std::optional<std::string> verify_array(std::string_view line);
    std::optional<std::string> verify_string(std::string_view line);
    std::optional<std::string> verify_number(std::string_view line);
    std::optional<std::string> verify_bool(std::string_view line);
    std::optional<std::string> verify_null(std::string_view line);

    // verify as-string json is valid
    std::optional<std::string> verify_json(std::string_view line);
}

#endif//__UTIL_HPP__
