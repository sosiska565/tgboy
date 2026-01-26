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
            bot.getApi().sendMessage(msg->chat->id, "Как тебя зовут?");
            state = UserState::WAIT_NAME;
            break;
        case UserState::WAIT_NAME:
            op_user->setName(msg->text);
            bot.getApi().sendMessage(msg->chat->id, "Твоя фамилия:");
            op_user->setState(UserState::WAIT_SECONDNAME);
            break;
        case UserState::WAIT_SECONDNAME:
            op_user->setSecondName(msg->text);
            bot.getApi().sendMessage(msg->chat->id, "Сколько тебе лет?");
            op_user->setState(UserState::WAIT_AGE);
            break;
        case UserState::WAIT_AGE:
            op_user->setAge(std::stoi(msg->text));
            bot.getApi().sendMessage(msg->chat->id, "Расскажи о себе");
            op_user->setState(UserState::WAIT_DESC);
            break;
        case UserState::WAIT_DESC:
            op_user->setDescription(msg->text);
            bot.getApi().sendMessage(msg->chat->id, "В каком ты городе?");
            op_user->setState(UserState::WAIT_CITY);
            break;
        case UserState::WAIT_CITY:
            op_user->setCity(msg->text);
            bot.getApi().sendMessage(msg->chat->id, "Ты женщина или мужчина?");
            op_user->setState(UserState::WAIT_GENDER);
            break;
        case UserState::WAIT_GENDER:
        bot.getApi().sendMessage(msg->chat->id, "Кого ты здесь ищешь? Мужчин или женщин?");
            op_user->setState(UserState::WAIT_SEARCHGENDER);
            break;
        case UserState::WAIT_SEARCHGENDER:
            //false = woman
            //true = man
            //костыль
            //вместо if(msg->text == "1")
            //нужно сделать 2 кнопки с выбором мужчины или женщины
            TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup);
            keyboard->resizeKeyboard = true;
            TgBot::KeyboardButton::Ptr button1(new TgBot::KeyboardButton);
            TgBot::KeyboardButton::Ptr button2(new TgBot::KeyboardButton);
            std::vector<TgBot::KeyboardButton::Ptr> row0;
            button1->text= "Мужской";
            row0.push_back(button1);
            button1->text= "Женский";
            row0.push_back(button2);

            keyboard->keyboard.push_back(row0);


            bot.getApi().sendMessage(msg->chat->id, "Анкета успешно создана!");
            op_user->setState(UserState::IDLE);
            createUser(op_user.value());
            return op_user.value();
            break;
    }

    createUser(op_user.value());

    return std::nullopt;
}
