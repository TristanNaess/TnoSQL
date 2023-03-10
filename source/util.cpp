#include "util.hpp"
#include <iostream>
#include <stdexcept> // replace with custom error header later

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
    inline bool is_hex(char c)
    {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
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
            throw std::runtime_error("util::match_quote() called with index != '\"'");
        }

        for (auto itr = line.begin() + quote + 1; itr != line.end(); itr++)
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

        size_t depth = 0; // can't start at one, because the first char is the bracket to be matched
        for (auto itr = line.begin() + opening_index; itr < line.end(); itr++)
        {
            if (*itr == open_char && *(itr - 1) != '\\') depth++;
            if (*itr == close_char && *(itr - 1) != '\\') depth--;
            if (depth == 0) return itr - line.begin();
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
        if (line.front() != '{') return "Object string missing opening brace";
        if (line.back() != '}') return "Object string missing closing brace";
        
        // strip surrounding braces and white space
        line.remove_prefix(1);
        line.remove_suffix(1);
        line = chomp(line);
 
        // break into fields
        std::vector<std::string_view> fields;
        size_t i = 0;
        while (line.size())
        {
            if (i == line.size())
            {
                fields.push_back(line);
                break;
            }
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
            if (f[0] == ':') return "Field is missing key";
            if (f[0] != '"') return "Key does not start with quote";

            std::string_view key, val;
            size_t delim;
            delim = match_quote(f, 0);
            delim = f.find(':', delim);
            if (delim == std::string_view::npos) return "':' delimiter missing between key and value";
            key = remove_trailing_ws(f.substr(0, delim));
            val = remove_leading_ws(f.substr(delim + 1));
            if (val.size() == 0) return "Field is missing value";

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
                case 'f':
                case 'T':
                case 'F':
                    result = verify_bool(val);
                    break;
                case 'n':
                case 'N':
                    result = verify_null(val);
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
        if (line.size() == 0) return "Array string has zero length";
        if (line.front() != '[') return "Array string missing opening brace";
        if (line.back() != ']') return "Array string missing closing brace";
        
        // strip surrounding braces and white space
        line.remove_prefix(1);
        line.remove_suffix(1);
        line = chomp(line);
 
        // break into fields
        std::vector<std::string_view> fields;
        size_t i = 0;
        while (line.size())
        {
            if (i == line.size())
            {
                fields.push_back(line);
                break;
            }
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

        for (const auto& f : fields)
        {
            if (f.size() == 0) return "Array contains empty field";

            std::optional<std::string> result;
            switch (f[0])
            {
                case '{':
                    result = verify_object(f);
                    break;
                case '[':
                    result = verify_array(f);
                    break;
                case '"':
                    result = verify_string(f);
                    break;
                case 't':
                case 'f':
                case 'T':
                case 'F':
                    result = verify_bool(f);
                    break;
                case 'n':
                case 'N':
                    result = verify_null(f);
                    break;
                default:
                    result = verify_number(f);
            }
            if (result) return "Error in field: " + result.value();
        }

        return {};
    }

    std::optional<std::string> verify_string(std::string_view line)
    {
        if (line.size() == 0) return "String is less than empty";
        if (line.front() != '"') return "String missing initial quote";
        if (line.back() != '"') return "String missing final quote";

        line.remove_prefix(1);
        line.remove_suffix(1);

        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] == '\\')
            {
                i++;
                switch (line[i])
                {
                    case '\\':
                    case '/':
                    case '"':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                        break;
                    case 'u':
                        i++;
                        if (!is_hex(line[i])) return "Non hex character in \\uXXXX escape sequence";
                        i++;
                        if (!is_hex(line[i])) return "Non hex character in \\uXXXX escape sequence";
                        i++;
                        if (!is_hex(line[i])) return "Non hex character in \\uXXXX escape sequence";
                        i++;
                        if (!is_hex(line[i])) return "Non hex character in \\uXXXX escape sequence";
                        break;
                    default:
                        return "Non escape character after '\\'";
                }
            }
            else if (line[i] == '"') return "Unescaped quote in string";
        }
        return {};
    }
    std::optional<std::string> verify_number(std::string_view line)
    {
        // verify no missing commas between number field and another
        if (line.find_first_of(WS_CHARS) != std::string_view::npos) return "Whitespace in number field, possibly missing field delimiter";
        try
        {
            std::stod(std::string(line));
        }
        catch(...)
        {
            return "Argument could not be parsed as a number";
        }
        return {};
    }
    std::optional<std::string> verify_bool(std::string_view line)
    {
        if (line == "true" || line == "false" || line == "True" || line == "False") return {};
        return "Argument does not match [Tt]rue or [Ff]alse";
    }
    std::optional<std::string> verify_null(std::string_view line)
    {
        if (line == "null" || line == "Null") return {};
        return "Argument does not match [Nn]ull";
    }

    std::optional<std::string> verify_json(std::string_view line)
    {
        return verify_object(line);
    }
}
