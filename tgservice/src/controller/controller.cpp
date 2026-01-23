#include "controller.h"
#include <cstdint>
#include <map>
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

std::map<int64_t, UserSession> sessions;

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

    TgBot::Bot bot(token);

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr msg){
        if (msg->text == "/start"){
            sessions[msg->from->id] = UserSession();
        }

        if(sessions.count(msg->from->id)){
            sessions[msg->from->id].next(bot, msg, sessions);
        } else {
            bot.getApi().sendMessage(msg->chat->id, "Чтобы начать /start");
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
