#include "util.hpp"
#include <iostream>
#include <stdexecept> // replace with custom error header later

// TODO: replace mixed throws and optionals with consistant optionals or errors, check runtime difference

namespace util
{
    const static std::string_view WS_CHARS = " \f\n\r\t\v";

    // local to .cpp file
    std::string_view remove_trailing_ws(std::string_view line)
    {
        size_t end = line.find_last_not_of(WS_CHARS);
        if (end == std::string_view::npos) return "";
        return line.substr(0, end + 1);
    }

    // local to .cpp file
    std::string_view remove_leading_ws(std::string_view line)
    {
        size_t start = line.find_first_not_of(WS_CHARS);
        if (start == std::string_view::npos) return "";
        return line.substr(start);
    }
    
    // local to .cpp file
    std::string_view chomp(std::string_view line) // may return empty string_view
    {
        return remove_trailing_ws(remove_leading_ws(line));
    }

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

    std::optional<std::string> verify_object(std::string_view line)
    {
        if (line.size() == 0) return "Object string has zero length";
        if (line.front() != '{') return "Object string missing opening bracket";
        if (line.back() != '}') return "Object string missing closing bracket";
        
        // strip surrounding braces and white space
        line.remove_prefix(1).remove_suffix(1);
        line = chomp(line);
 
        // break into fields
        std::vector<std::string_view> fields;
        size_t i = 0;
        while (line.size() > 0)
        {
            switch (line[i])
            {
                case '[': // skip between brackets
                case '{':
                    i = match_bracket(line, i);
                    break;
                case '"': // skip string
                    i = match_quote(line, i);
                    break;
                case ',': // consume contents
                    fields.push_back(remove_trailing_ws(line.substr(0, i)));
                    line = remove_leading_ws(line.substr(i + 1));
                    i = 0;
                    continue; // skip increment
            }
            i++;
        }

        // verify fields
        for (const auto& f : fields)
        {
            if (f.size() == 0) return "Object contains empty field";
            if (f[0] != '"') return "Key does not start with quote";

            std::string_view key, val;
            size_t delim;
            delim = match_quote(f);
            delim = f.find(':', delim);
            key = remove_trailing_ws(f.substr(0, delim));
            if (key.size() == 0) return "Key has zero length";
            val = remove_leading_ws(f.substr(delim + 1));
            if (val.size() == 0) return "Value has zero length";

            auto result = verify_string(key);
            if (result) return "Error in Key: " + result.value();
            
            switch (val[0])
            {
                case '{':
                    result = verify_object(val);
                    break;
                case '[':
                    result = verify_array(val);
                    break;
                case '"':
                    result = verify_string(val);
                    break;
                case 't':
                case 'T':
                case 'f':
                case 'F':
                    result = verify_bool(val);
                    break;
                case 'n':
                case 'N':
                    result = verfify_null(val);
                    break;
                default:
                    result = verify_number(val);
            }
            if (result) return "Error in value: " + result.value();
        }

        return {};
    }

    std::optional<std::string> verify_array(std::string_view line)
    {
        return "TODO: implement util::verify_array()";
    }

    std::optional<std::string> verify_string(std::string_view line)
    {
        return "TODO: implement util::verify_string()";
    }
    std::optional<std::string> verify_number(std::string_view line)
    {
        return "TODO: implement util::verify_number()";
    }
    std::optional<std::string> verify_bool(std::string_view line)
    {
        return "TODO: implement util::verify_bool()";
    }
    std::optional<std::string> verify_null(std::string_view line)
    {
        return "TODO: implement util::verify_null()";
    }

    std::optional<std::string> verify_json(std::string_view line)
    {
        return verify_object(line);
    }
}
