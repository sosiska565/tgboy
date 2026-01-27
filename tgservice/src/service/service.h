#ifndef SERVICE_H
#define SERVICE_H

#include <optional>
#include <tgbot/tgbot.h>
#include "../repository/user/user.h"

class Service{
    public:
        User createUser(User &user);
        std::optional<User> getUserById(int64_t id);
        std::optional<User> createAncete(int64_t id, TgBot::Bot &bot, TgBot::Message::Ptr msg);
};

#endif
