#include "controller.h"
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <tgbot/tgbot.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include "../service/service.h"
#include "../repository/user/user.h"

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void Controller::runBot(){
    std::ifstream tokensFile("../tokens.txt");
    std::string tokensLine;
    std::string token;

    if(tokensFile.is_open()){
        while(std::getline(tokensFile, tokensLine)) {
            std::vector tokens = split(tokensLine, '=');
            if(tokens[0] != "tg_token") continue;

            token = tokens[1];
        }
    }

    if(token.empty()) throw "Error: token is empty";

    Service service;
    TgBot::Bot bot(token);

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr msg){
        std::optional<User> op_user = service.getUserById(msg->from->id);

        if (msg->text == "/start") {
            if (!op_user.has_value()) {
                std::cout << "Creating a new user..." << std::endl;
                User u;
                u.setTgId(msg->from->id);
                u.setState(UserState::WAIT_NAME);
                service.createUser(u);
                bot.getApi().sendMessage(msg->chat->id, "Привет! Как тебя зовут?");
            } else {
                std::string ancete;

                ancete =    std::string("Твоя анкета\n") + 
                            "Имя: " + op_user->getName() + "\n" +
                            "Фамилия: " + op_user->getSecondName() + "\n" +
                            "Возвраст: " + std::to_string(op_user->getAge()) + "\n" +
                            "Описание: " + op_user->getDescription() + "\n" +
                            "Город: " + op_user->getCity() + "\n";

                bot.getApi().sendMessage(
                    msg->chat->id,
                    ancete 
                );
            }
            return;
        }

        if (op_user.has_value() && op_user->getState() != UserState::IDLE) {
            service.createAncete(msg->from->id, bot, msg);
        } else {
            bot.getApi().sendMessage(msg->chat->id, "Чтобы начать, напиши /start");
        } 
    });

    try{
        TgBot::TgLongPoll longPoll(bot);
        
        while(true){
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        std::cerr << "Runtime error: " << e.what() << " " << std::endl; 
    }    
}
