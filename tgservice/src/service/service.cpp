#include "service.h"
#include <thread>
#include <tgbot/tgbot.h>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

void Service::createUser(TgBot::Bot &bot, TgBot::Message::Ptr message){
    std::thread([&bot, message](){

    }).detach();
}
