#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <cstdint>
#include <pqxx/pqxx>
#include "user.h"

class UserRepository{
    private:
        pqxx::connection conn;

    public:
        UserRepository(const std::string& conn_str);
        User saveUser(User& user);
        std::vector<User> getUsers();
        std::optional<User> getUserById(int64_t id);
};

#endif
