#ifndef USER_H
#define USER_H

#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>
#include <tgbot/tgbot.h>

enum class UserState{
    IDLE,
    WAIT_NAME,
    WAIT_SECONDNAME,
    WAIT_AGE,
    WAIT_DESC,
    WAIT_CITY,
    WAIT_GENDER,
    WAIT_SEARCHGENDER
};

class UserSession{
    public:
        UserState state = UserState::IDLE;
        nlohmann::json data;

        void next(TgBot::Bot& bot, TgBot::Message::Ptr msg, std::map<int64_t, UserSession>& sessions){
            switch(state){
                case UserState::IDLE:
                    bot.getApi().sendMessage(msg->chat->id, "Как тебя зовут?");
                    state = UserState::WAIT_NAME;
                    break;
                case UserState::WAIT_NAME:
                    data["name"] = msg->text;
                    bot.getApi().sendMessage(msg->chat->id, "Твоя фамилия:");
                    state = UserState::WAIT_SECONDNAME;
                    break;
                case UserState::WAIT_SECONDNAME:
                    data["secondname"] = msg->text;
                    bot.getApi().sendMessage(msg->chat->id, "Сколько тебе лет?");
                    state = UserState::WAIT_DESC;
                    break;
                case UserState::WAIT_AGE:
                   data["age"] = msg->text;
                   bot.getApi().sendMessage(msg->chat->id, "Расскажи о себе");
                   state = UserState::WAIT_DESC;
                   break;
                case UserState::WAIT_DESC:
                   data["discription"] = msg->text;
                   bot.getApi().sendMessage(msg->chat->id, "В каком ты городе?");
                   state = UserState::WAIT_CITY;
                   break;
                case UserState::WAIT_CITY:
                   data["city"] = msg->text;
                   bot.getApi().sendMessage(msg->chat->id, "Ты женщина или мужчина?");
                   state = UserState::WAIT_GENDER;
                   break;
                case UserState::WAIT_GENDER:
                   data["gender"] = msg->text;
                   bot.getApi().sendMessage(msg->chat->id, "Кого ты здесь ищешь? Мужчин или женщин?");
                   state = UserState::WAIT_SEARCHGENDER;
                   break;
                case UserState::WAIT_SEARCHGENDER:
                   data["searchgender"] = msg->text;
                   bot.getApi().sendMessage(msg->chat->id, "Анкета успешно создана!");
                   state = UserState::IDLE;
                   sessions.erase(msg->from->id);
                   break;
            }
        }
};

class User {
    private:
        int tgid;
        int age;
        std::string name;
        std::string secondname;
        std::string description;
        std::string city;
        bool isSearching;
        bool isSearchingGender;
        bool gender;
        UserState state = UserState::IDLE;

    public:
        int getTgId(){
            return tgid;
        }

        int getAge(){
            return age;
        }

        std::string getName(){
            return name;
        }

        std::string getSecondName(){
            return secondname;
        }

        std::string getDescription(){
            return description;
        }

        std::string getCity(){
            return city;
        }

        bool getIsSearching(){
            return isSearching;
        }

        bool getIsSearchingGender(){
            return isSearchingGender;
        }

        bool getGender(){
            return gender;
        }

        UserState getState(){
            return state;
        }

        //setters

        void setTgId(int tgid){
            this->tgid = tgid;
        }

        void setAge(int age){
            this->age = age;
        }

        void setName(std::string name){
            this->name = name;
        }

        void setSecondName(std::string secondname){
            this->secondname = secondname;
        }

        void setDescription(std::string description){
            this->description = description;
        }

        void setCity(std::string city){
            this->city = city;
        }

        void setIsSearchingGender(bool isSearchingGender){
            this->isSearchingGender = isSearchingGender;
        }

        void setIsSearching(bool isSearching){
            this->isSearching = isSearching;
        }

        void setGender(bool gender){
            this->gender = gender;
        }

        void setState(UserState state){
            this->state = state;
        }
};

#endif
