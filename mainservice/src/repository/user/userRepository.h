#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <cstdint>
#include <pqxx/pqxx>

class UserRepository{
    private:
        pqxx::connection conn;

    public:
        UserRepository(const std::string& conn_str);
        void saveUser(
            int64_t id,
            const std::string& name,
            const std::string& secondname,
            const std::string& description,
            const std::string& city,
            int age,
            bool isSearching,
            bool isSearchingGender
        );
};

#endif
