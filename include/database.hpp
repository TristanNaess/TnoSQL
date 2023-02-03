#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

#include "collection.hpp"
#include "document.hpp"

#include <unordered_map>
#include <vector>
#include <initializer_list>
#include <string>


// database can be constructed empty with constructor 1 or populated with collections by name, either as a vector (2) or initializer list (3)
// added collections populate the documents, with repeated  IDs cross-referenced rather than re-created

class Database
{
    public:
        Database(Memory mode); // ------------------------------------------------- 1
        Database(Memory mode, const std::vector<std::string>& collections); // ---- 2
        Database(Memory mode, std::initializer_list<std::string> collections); // - 3

        // All interface is done through the Database class
        // Create
        insert(const std::string& document_data, const std::string& collection); // add to single collection
        insert(const std::string& document_data, const std::vector<std::string>& collections); // add to multiple collections
        insert(const std::string& document_data, std::initializer_list<std::string> collections); // add to multiple collections

        // Read/Update
        // decide whether documents can be directly accessed or only by field

        // Delete
        remove(size_t id);
        remove(const std::vector<size_t>& ids);
        remove(std::initializer_list<size_t> ids);

    private:
        std::unordered_map<size_t, Document> documents;
        std::unordered_map<std::string, Collection> collections;
};

#endif//__DATABASE_HPP__
