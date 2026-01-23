#include "service.h"
#include "../repository/user/user.h"
#include "../repository/user/userRepository.h"
#include <cstdint>
#include <optional>
#include <stdexcept>

std::string url = "host=localhost port=5432 dbname=sparktreat user=spark password=admin";

std::vector<User> Service::getUsers(){
    UserRepository repo(url);

    return repo.getUsers();
}

User Service::createUser(User& user){
    UserRepository repo(url);

    User newUser = repo.saveUser(user);

    return newUser;
}

User Service::updateUser(User& user){
    UserRepository repo(url);

    User newUser = repo.saveUser(user);

    return newUser;
}

std::optional<User> Service::getUserById(int64_t id){
    UserRepository repo(url);

    std::optional<User> op_user = repo.getUserById(id);

    if(op_user == std::nullopt){
        throw std::runtime_error("User by id not found.");
    }

    return op_user;
}
