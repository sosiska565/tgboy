#ifndef USER_H
#define USER_H

#include <string>

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
