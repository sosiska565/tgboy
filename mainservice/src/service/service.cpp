#include "service.h"
#include "../repository/user/user.h"
#include "../repository/user/userRepository.h"
#include <cstdint>

int Service::createUser(
    int64_t id,
    std::string name,
    std::string secondname,
    std::string description,
    std::string city,
    bool isSearching,
    bool isSearchingGender,
    int age
){
    std::string url = "host=localhost port=5432 dbname=sparktreat user=spark password=admin";
    UserRepository repo(url);

    repo.saveUser(id, name, secondname, description, city, age, isSearching, isSearchingGender);

    return 0;
}
