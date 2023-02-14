#include "util.hpp"
#include <iostream>
#include <stdexecept> // replace with custom error header later

namespace util
{

    // local to .cpp file
    size_t match_quote(std::string_view line, size_t quote)
    {
        if (line.size() <= quote)
        {
            // this should never happen, since this function is internal to the library
            // but this works for now. Replace later with assert or exaustively verify check unnecessary
            throw std::runtime_error("util::match_quote() called with index past end of string");
        }
        if (line[quote] != '"')
        {
            // same here, replace later
            throw std::runtime_erro("util::match_quote() called with index != '\"'");
        }

        for (const auto itr = line.begin() + quote + 1; itr != line.end(); itr++)
        {
            if (*itr == '"' && *(itr - 1) != '\\') return itr - line.begin();
        }

        // this can happen for bad input
        throw std::runtime_error("No matching quote");
    }

    // local to .cpp file
    size_t match_bracket(std::string_view line, size_t opening_index)
    {
        // same comments as util::match_quote()
        if (line.size() <= opening_index)
        {
            throw std::runtime_error("util::match_bracket() called with index past end of string");
        }

        char open_char, close_char;
        switch (line[opening_index])
        {
            // ( and < ignored because not part of syntax
            case '[':
                open_char = '[';
                close_char = ']';
                break;
            case '{':
                open_char = '{';
                close_char = '}';
                break;
            default:
                throw std::runtime_error("util::match_bracket() called with index != [ or {");
        }

        size_t depth = 1;
        for (const auto itr = line.begin() + opening_index; itr < line.end(); itr++)
        {
            if (*itr == open_char && *(itr - 1) != '\\') depth++;
            if (*itr == close_char && *(itr - 1) !+ '\\') depth--;
            if (depth == 0) return *itr - line.begin();
        }

        throw std::runtime_error("No matching bracket");
    }

    std::unordered_map<std::string, Data> build_structure(const std::string& data)
    {
        std::cerr << "TODO: implment build_structure\n";
        std::unordered_map<std::string, Data> obj;
        return obj;
    }

    std::string_view extract_object_string(std::string_view line)
    {
        throw std::runtime_error("TODO: implement util::extract_object_string()");
    }

    std::string_view extract_array_string(std::string_view line)
    {
        throw std::runtime_error("TODO: implement util::extract_array_string()");
    }
    
    std::string_view extract_object_field(std::string_view line, std::string_view key)
    {
        throw std::runtime_error("TODO: implement util::extract_object_field()");
    }

    std::string_view extract_array_field(std::string_view line, size_t index)
    {
        throw std::runtime_error("TODO: implement util::extract_array_field()");
    }

    bool verify_object(std::string_view line)
    {
        throw std::runtime_error("TODO: implement util::verify_object()");
    }

    bool verify_array(std::string_view line)
    {
        throw std::runtime_error("TODO: implement util::verify_array()");
    }

    bool verify_json(std::string_view line)
    {
        return verify_object(line);
    }
}
