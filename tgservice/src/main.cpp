#include "controller/controller.h"
#include <iostream>

int main(int argc, char **argv){
    try{
        Controller con;
        con.runBot();
    }
    catch (char *e) {
        std::cout << e << std::endl;
    }

    return 0;
}
