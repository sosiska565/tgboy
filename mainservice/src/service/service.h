#ifndef SERVICE_H
#define SERVICE_H

#include "../repository/user/user.h"
#include "../repository/user/userRepository.h"
#include <cstdint>

class Service{
    public:
        int createUser(
            int64_t id,
            std::string name,
            std::string secondname,
            std::string description,
            std::string city,
            bool isSearching,
            bool isSearchingGender,
            int age
        );
};

#endif
