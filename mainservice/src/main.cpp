#include <crow/app.h>
#include <iostream>
#include <crow.h>

int main(int argc, char **argv){
    crow::SimpleApp app;
    
    

    CROW_ROUTE(app, "/")([](){
            return "Hello world";
    });

    app.port(8080).run();
    return 0;
}
