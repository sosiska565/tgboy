#ifndef SERVICE_H
#define SERVICE_H

#include <tgbot/tgbot.h>
#include <string>
#include <vector>
#include <sstream>

class Service{
    private:
        static std::vector<std::string> split(const std::string& s, char delimiter);
    public:
        static void startBot();
};

#endif
