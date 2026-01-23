#include "controller.h"
#include <crow.h>
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/json.h>
#include "../service/service.h"

void Controller::runController(){
    Service service;
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api/user/ancete").methods(crow::HTTPMethod::POST)
    ([&service](const crow::request& req){
        auto x = crow::json::load(req.body);

        if(!x) return crow::response(400, "Invalid JSON");

        service.createUser(x["tgid"].i(), x["name"].s(), x["secondname"].s(), x["description"].s(), x["city"].s(), x["isSearching"].b(), x["isSearchingGender"].b(), x["age"].i());

        crow::json::wvalue res;
        res["status"] = "OK";
        res["statusCode"] = 200;

        return crow::response(res);
    });

    app.port(8080).multithreaded().run();
}
