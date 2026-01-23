#ifndef SERVICE_H
#define SERVICE_H

#include <tgbot/tgbot.h>

class Service{
    public:
        void createUser(TgBot::Bot &bot, TgBot::Message::Ptr message);
};

#endif
