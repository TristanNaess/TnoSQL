#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

#include "collection.hpp"
#include "document.hpp"

#include <unordered_map>
#include <initializer_list>
#include <string>

// -----------------
//  Modes
// -----------------
// Structured memory represents a document as a map in memory
// Parsed memory stores a document as a (utf-8) string and parses to find fields as requested
// Temporary stores a number of documents in memory fully parsed, and as necessary cycles recently referenced documents into a temp file for storage


// database can be constructed empty with constructor 1 or populated with collections by name, either as a vector or initializer list
// added collections populate the documents, with repeated  IDs cross-referenced rather than re-created

enum class Memory {Structured, Parsed, Temporary};

class Database
{
    public:
        Database(Memory mode); // ----------------------------------------------- 1
        Database(Memory mode, std::vector<std::string> collections); // --------- 2
        Database(Memory mode, std::initializer_list<std::string> collections); // 3

        // All interface is done through the Database class
        // Create
        insert(std::string document_data, std::string collection);
        insert(std::string document_data, std::vector<std::string> collections);
        insert(std::string document_data, std::initializer_list<std::string> collections);

        // Read/Update
        // decide whether documents can be directly accessed or only by field

        // Delete
        remove(size_t id);
        remove(std::vector<size_t> ids);
        remove(std::initializer_list<size_t> ids);

    private:
        std::unordered_map<size_t, Document> documents;
        std::unordered_map<std::string, Collection> collections;
};

#endif//__DATABASE_HPP__
