#include "mongodb-tools/mongodb.h"

int main(){

    std::string server = "127.0.0.1";
    std::string user = "root";
    std::string pwd = "E4!3%T0!JsfXl9m7yQLN7yC^Y";
    //std::string db = "admin";

    mongodb MyObj;
    mongodb MyObj0;
    mongodb::DataBaseConfigs config;
    config.Server = server;
    config.username = user;
    config.pwd = pwd;

    MyObj.init(config);
    MyObj0.init(config);

    /*std::cout << "hi\n";
    std::vector<std::string> list = MyObj.ListDBs();

    for (std::string item : list){
        std::cout << item << std::endl;
    }*/

    std::vector<std::map<std::string , std::string>> map = MyObj.findDocumentsMap("aid" , "profiles");
    std::vector<std::map<std::string , std::string>> map0 = MyObj0.findDocumentsMap("aid" , "profiles");

    //MyObj.~mongodb();

    std::cout << "bye" << std::endl;

    return 0;
}
