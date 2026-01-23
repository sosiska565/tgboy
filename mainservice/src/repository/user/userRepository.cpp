#include "userRepository.h"
#include <cstdint>
#include <string>

UserRepository::UserRepository(const std::string& conn_str) : conn(conn_str){
    pqxx::work txn(conn);

    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id BIGINT PRIMARY KEY,
            name TEXT,
            secondName TEXT,
            description TEXT,
            city TEXT,
            isSearching BOOLEAN,
            isSearchingGender BOOLEAN,
            age INTEGER
        );
    )");
    txn.commit();
}

void UserRepository::saveUser(
    int64_t id,
    const std::string& name,
    const std::string& secondname,
    const std::string& description,
    const std::string& city,
    int age,
    bool isSearching,
    bool isSearchingGender
){
    pqxx::work txn(conn);

    txn.exec_params(
            "INSERT INTO users (id, name, secondName, description, isSearching, age, city, isSearchingGender) VALUES ($1, $2, $3, $4, $5, $6)"
            "ON CONFLICT (id) DO UPDATE SET name = $2, secondName = $3, description = $4, isSearching = $5, age = $6, city = $7, isSearchingGender = $8",
            id, name, secondname, description, isSearching, age, city, isSearchingGender
    );

    txn.commit();
}
