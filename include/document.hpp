#ifndef __DOCUMENT_HPP__
#define __DOCUMENT_HPP__

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

// -----------------
//  Modes
// -----------------
// Structured memory represents a document as a map in memory
// Parsed memory stores a document as a (utf-8) string and parses to find fields as requested
// Temporary stores a number of documents in memory fully parsed, and as necessary cycles recently referenced documents into a temp file for storage

enum class DataType { Object, Array, String, Number, Boolean, Null };

class Data
{
    public:
        Data(const std::string& data);

        // probably replace with a templated function later
        const std::string& get_string() const;
        std::string& get_string();
        const double& get_number() const;
        double& get_number();
        // see if number can be split into long and double to better store depending on type
        const bool& get_boolean() const;
        bool& get_boolean();

        const Data& get(size_t index) const;
        Data& get(size_t index);
        const Data& get(const std::string& key) const;
        Data& get(const std::string& key);

    private:
        DataType type;
        std::variant<
            std::unordered_map<std::string, Data>,
            std::vector<Data>, // possibly replace with unordered_map, if ref invalidation is a concern
            std::string,
            double,
            bool> // also used for Null
        m_data;
};

/*
// return wrapper class to allow return of Object and Array data without allowing assignment
class DataReturnWrapper
{
    public:
        DataReturnWrapper()

    private:
};
*/

enum class Memory {Structured, Parsed, Temporary};

class Document
{
    public:
        Document(Memory mode, std::string data);

    private:
        std::variant<std::string, std::unordered_map<std::string, Data>> m_data;
};

#endif//__DOCUMENT_HPP__
