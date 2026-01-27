#include "controller.h"
#include <cstdint>
#include <exception>
#include <map>
#include <optional>
#include <string>
#include <tgbot/tgbot.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <tgbot/types/CallbackQuery.h>
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
                std::cout << u.getTgId() << std::endl;
                service.createUser(u);
                bot.getApi().sendMessage(msg->chat->id, "Твое имя: ");
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

    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query){
        std::optional<User> op_user = service.getUserById(query->from->id);    
    
        if(op_user.has_value()){
            if(query->data == "isSearchingGender = female"){
                op_user->setIsSearchingGender(false);
                op_user->setState(UserState::WAIT_SEARCHGENDER);
                service.createUser(op_user.value());
                service.createAncete(query->from->id, bot, query->message);
            }
            else if(query->data == "isSearchingGender = male"){
                op_user->setIsSearchingGender(true);
                op_user->setState(UserState::WAIT_SEARCHGENDER);
                service.createUser(op_user.value());
                service.createAncete(query->from->id, bot, query->message);
            }
            else if(query->data == "gender = female"){
                op_user->setGender(false);
                op_user->setState(UserState::WAIT_GENDER);
                service.createUser(op_user.value());
                service.createAncete(query->from->id, bot, query->message);
            }
            else if(query->data == "gender = male"){
                op_user->setGender(true);
                op_user->setState(UserState::WAIT_GENDER);
                service.createUser(op_user.value());
                service.createAncete(query->from->id, bot, query->message);
            }
        }
        try {
            bot.getApi().answerCallbackQuery(query->id);
        }
        catch (std::exception ex){
            std::cout << ex.what() << std::endl;
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
