#include "service.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <thread>

std::vector<std::string> Service::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void Service::startBot(){
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

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message){
        bot.getApi().sendMessage(message->chat->id, "Привет! Я SparkTreat, бот для знакомств!\nТы можешь:\n1. /start");
    });

    bot.getEvents().onCommand("test", [&bot](TgBot::Message::Ptr message){
        std::thread([](){
            nlohmann::json body;
        }).detach();
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
