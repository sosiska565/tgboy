#include "service.h"
#include <cpr/api.h>
#include <cpr/body.h>
#include <cpr/cprtypes.h>
#include <cpr/response.h>
#include <cstddef>
#include <cstdint>
#include <nlohmann/detail/string_utils.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <tgbot/Bot.h>
#include <thread>
#include <tgbot/tgbot.h>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <tgbot/types/KeyboardButton.h>

std::string url = "http://127.0.0.1:8080";

User Service::createUser(User &user){
    std::string newUrl = url + "/api/user/ancete";

    nlohmann::json j = user;

    cpr::Response r = cpr::Post(
        cpr::Url{newUrl},
        cpr::Body{j.dump()},
        cpr::Header{{"Content-Type", "application/json"}}
    );

    std::cout << j.dump() << std::endl;

    if(r.status_code != 200){
        std::cout << "Status code: " + std::to_string(r.status_code) << std::endl;
    }
    return user;
}

std::optional<User> Service::getUserById(int64_t id) {
    std::string newUrl = url + "/api/user/ancete/" + std::to_string(id);

    cpr::Response r = cpr::Get(cpr::Url{newUrl});

    if (r.status_code == 200) {
        try {
            auto j = nlohmann::json::parse(r.text);
            return j.get<User>();
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] JSON Parse error: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "[ERROR] getUserById HTTP status: " << r.status_code << std::endl;
    }

    return std::nullopt;
}

std::optional<User> Service::createAncete(int64_t id, TgBot::Bot& bot, TgBot::Message::Ptr msg){
    std::optional op_user = getUserById(id);

    if(op_user == std::nullopt) throw std::runtime_error("User doesnt exists");

    UserState state = op_user->getState();

    switch(state){
        case UserState::IDLE:
            bot.getApi().sendMessage(msg->chat->id, "Напиши своё имя: ");
            state = UserState::WAIT_NAME;
            break;
        case UserState::WAIT_NAME:
            op_user->setName(msg->text);
            bot.getApi().sendMessage(msg->chat->id, "Напиши свою фамилию: ");
            op_user->setState(UserState::WAIT_SECONDNAME);
            break;
        case UserState::WAIT_SECONDNAME:
            op_user->setSecondName(msg->text);
            bot.getApi().sendMessage(msg->chat->id, "Напиши свой возраст: ");
            op_user->setState(UserState::WAIT_AGE);
            break;
        case UserState::WAIT_AGE:
            op_user->setAge(std::stoi(msg->text));
            bot.getApi().sendMessage(msg->chat->id, "Напиши информацию о себе: ");
            op_user->setState(UserState::WAIT_DESC);
            break;
        case UserState::WAIT_DESC:
            op_user->setDescription(msg->text);
            bot.getApi().sendMessage(msg->chat->id, "Напиши свой город: ");
            op_user->setState(UserState::WAIT_CITY);
            break;
        case UserState::WAIT_CITY: {
            op_user->setCity(msg->text);

            TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

            std::vector<TgBot::InlineKeyboardButton::Ptr> row1;

            TgBot::InlineKeyboardButton::Ptr maleBtn(new TgBot::InlineKeyboardButton);
            maleBtn->text = "Мужчина";
            maleBtn->callbackData = "gender = male";

            TgBot::InlineKeyboardButton::Ptr femaleBtn(new TgBot::InlineKeyboardButton);

            femaleBtn->text = "Женщина";
            femaleBtn->callbackData = "gender = female";

            row1.push_back(maleBtn);
            row1.push_back(femaleBtn);
            keyboard->inlineKeyboard.push_back(row1);

            bot.getApi().sendMessage(msg->chat->id, "Твой пол", nullptr, nullptr, keyboard);
            break;
        }
        case UserState::WAIT_GENDER: {
            TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

            std::vector<TgBot::InlineKeyboardButton::Ptr> row1;

            TgBot::InlineKeyboardButton::Ptr maleBtn(new TgBot::InlineKeyboardButton);
            maleBtn->text = "Мужчин";
            maleBtn->callbackData = "isSearchingGender = male";

            TgBot::InlineKeyboardButton::Ptr femaleBtn(new TgBot::InlineKeyboardButton);
            femaleBtn->text = "Женщин";
            femaleBtn->callbackData = "isSearchingGender = female";

            row1.push_back(maleBtn);
            row1.push_back(femaleBtn);
            keyboard->inlineKeyboard.push_back(row1);

            bot.getApi().sendMessage(msg->chat->id, "Выбери кого ты ищешь", nullptr, nullptr, keyboard);
            break;
        }
        case UserState::WAIT_SEARCHGENDER:
            
            bot.getApi().sendMessage(msg->chat->id, "Анкета успешно создана!");
            op_user->setState(UserState::IDLE);
            createUser(op_user.value());
            return op_user.value();
            break;
    }

    createUser(op_user.value());

    return std::nullopt;
}
