#include "controller.h"
#include <crow.h>
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/json.h>
#include "../service/service.h"
#include "../repository/user/user.h"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>

void Controller::runController(){
    Service service;
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api/user/ancete").methods(crow::HTTPMethod::GET)
    ([&service](){
        nlohmann::json user = service.getUsers();

        crow::response res;
        res.set_header("Content-Type", "application/json");
        res.body = user.dump();
        return res;
    });

    CROW_ROUTE(app, "/api/user/ancete/<int>").methods(crow::HTTPMethod::GET)
    ([&service](int64_t id){
        std::optional<User> op_user = service.getUserById(id);

        if(op_user == std::nullopt){
            return crow::response(404, "Not found.");
        }

        nlohmann::json user = op_user; 

        crow::response res;
        res.set_header("Content-Type", "application/json");
        res.body = user.dump();
        return res;
    });

    CROW_ROUTE(app, "/api/user/ancete").methods(crow::HTTPMethod::POST)
    ([&service](const crow::request& req){
        auto x = crow::json::load(req.body);

        if(!x) return crow::response(400, "Invalid JSON");

        User user;

        user.setTgId(x["id"].i());
        user.setAge(x["age"].i());
        user.setName(x["name"].s());
        user.setSecondName(x["secondname"].s());
        user.setDescription(x["description"].s());
        user.setCity(x["city"].s());
        user.setIsSearching(x["isSearching"].b());
        user.setIsSearchingGender(x["isSearchingGender"].b());
        user.setGender(x["gender"].b());
        user.setState((UserState)x["state"].i());

        nlohmann::json newUser = service.createUser(user);

        crow::response res;
        res.set_header("Content-Type", "application/json");
        res.body = newUser.dump();
        return res;
    });

    app.port(8080).multithreaded().run();
}
