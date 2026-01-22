#include "service/service.h"
#include <iostream>

int main(int argc, char **argv){
    try{
        Service::startBot();   
    }
    catch (char *e) {
        std::cout << e << std::endl;
    }

    return 0;
}
