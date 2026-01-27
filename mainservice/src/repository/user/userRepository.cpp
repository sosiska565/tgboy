#include "userRepository.h"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "user.h"
#include <iostream>

UserRepository::UserRepository(const std::string& conn_str) : conn(conn_str){
    pqxx::work txn(conn);

    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id BIGINT PRIMARY KEY,
            age INTEGER,
            name TEXT,
            secondname TEXT,
            description TEXT,
            city TEXT,
            isSearching BOOLEAN,
            isSearchingGender BOOLEAN,
            gender BOOLEAN,
            state INTEGER
        );
    )");
    txn.commit();
}

std::vector<User> UserRepository::getUsers(){
    pqxx::work txn(conn);
    pqxx::result res = txn.exec(R"(
        SELECT
        id,
        age,
        name,
        secondname,
        description,
        city,
        isSearching,
        isSearchingGender,
        gender,
        state
        FROM users
    )");

    std::vector<User> users;
    for(auto const &row : res){
        User u;
        u.setTgId(row["id"].as<int64_t>());
        u.setAge(row["age"].as<int>());
        u.setName(row["name"].as<std::string>());
        u.setSecondName(row["secondname"].as<std::string>());
        u.setDescription(row["description"].as<std::string>());
        u.setCity(row["city"].as<std::string>());
        u.setIsSearching(row["isSearching"].as<bool>());
        u.setIsSearchingGender(row["isSearchingGender"].as<bool>());
        u.setGender(row["gender"].as<bool>());
        u.setState((UserState)row["state"].as<int>());
    }

    return users;
}

User UserRepository::saveUser(User& user){
    pqxx::work txn(conn);
    
    std::cout << "USER ID: " << user.getTgId() << std::endl;

    txn.exec_params(
        R"(
            INSERT INTO users (
                id,
                age,
                name, 
                secondname, 
                description,
                city,
                isSearching,
                isSearchingGender,
                gender,
                state
            ) VALUES (
                $1, $2, 
                $3, $4, 
                $5, $6,
                $7, $8,
                $9, $10
            )
            ON CONFLICT (id) DO UPDATE SET 
                age = $2,
                name = $3, 
                secondname = $4, 
                description = $5, 
                city = $6,  
                isSearching = $7, 
                isSearchingGender = $8,
                gender = $9,
                state = $10
        )",
            user.getTgId(), 
            user.getAge(),
            user.getName(),
            user.getSecondName(),
            user.getDescription(),
            user.getCity(),
            user.getIsSearching(),
            user.getIsSearchingGender(),
            user.getGender(),
            (int)user.getState()
    );

    txn.commit();

    std::cout << "User created." << std::endl;

    return user;
}

std::optional<User> UserRepository::getUserById(int64_t id){
    pqxx::work txn(conn);

    pqxx::result res = txn.exec(
    R"(
        SELECT * FROM users WHERE id = $1
    )", pqxx::params{
        id
    });

    if(res.empty()){
        return std::nullopt;
    }

    auto const row = res[0];

    User u;
    u.setTgId(row["id"].as<int64_t>());
    u.setAge(row["age"].as<int>());
    u.setName(row["name"].as<std::string>());
    u.setSecondName(row["secondname"].as<std::string>());
    u.setDescription(row["description"].as<std::string>());
    u.setCity(row["city"].as<std::string>());
    u.setIsSearching(row["isSearching"].as<bool>());
    u.setIsSearchingGender(row["isSearchingGender"].as<bool>());
    u.setGender(row["gender"].as<bool>());
    u.setState(static_cast<UserState>(row["state"].as<int>()));

    return u;
}
