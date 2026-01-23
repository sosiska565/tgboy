#ifndef SERVICE_H
#define SERVICE_H

#include "../repository/user/user.h"
#include "../repository/user/userRepository.h"
#include <cstdint>

class Service{
    public:
        User createUser(User& user);
        User updateUser(User& user);
        std::vector<User> getUsers();
        std::optional<User> getUserById(int64_t id);
};

#endif
