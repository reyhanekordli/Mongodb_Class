#ifndef MONGODB_H
#define MONGODB_H

#include<cstdint>
#include<iostream>
#include<vector>
#include<boost/algorithm/string.hpp>
#include<string>
#include<map>

#include<bsoncxx/builder/basic/document.hpp>
#include<bsoncxx/json.hpp>
#include<bsoncxx/stdx/string_view.hpp>
#include<bsoncxx/types/value.hpp>
#include<bsoncxx/types.hpp>

#include<mongocxx/pool.hpp>
#include<mongocxx/instance.hpp>
#include<mongocxx/stdx.hpp>
#include<mongocxx/uri.hpp>
#include<mongocxx/client.hpp>
#include<mongocxx/database.hpp>


using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

class mongodb
{
public:

    struct DataBaseConfigs
    {
        std::string Server = "";
        std::string username = "";
        std::string pwd = "";
        int tls = 1;
        std::string port = "27017";
    };

    mongodb();

    ~mongodb();

    std::string ConvertToDate(std::string value);

    int init(mongodb::DataBaseConfigs Configs);

    //void CreateConnection();
    void isConnectionAlive();

    //void CreateDB(std::string DBName);
    bool isDBExisting(std::string DBName);
    void DropDB(std::string DBName);

    void CreateDBandCollection(std::string DBName , std::string CollName);
    bool isCollectionExisting(std::string DBName , std::string CollName);
    void DropCollection(std::string DBName , std::string CollName);

    std::vector<std::string> ListDBs();
    std::vector<std::string> ListCollections(std::string DBName);

    std::vector<std::string> findDocuments(std::string DBName , std::string CollName);
    //std::vector<std::map<std::string , std::string>> findDocumentsMap(std::string DBName , std::string CollName);

    int InsertDocument(std::string DBName , std::string CollName , std::string json_input);

    void DeleteAllDocuments(std::string DBName , std::string CollName);

    bool UpdateDocuments(std::string DBName , std::string CollName , std::string new_input , std::string specifier);

    void PrintAllDocuments(std::string DBName , std::string CollName);

    void QueryDirectlyInDB(std::string DBName , std::string Conditions);

    std::vector<std::map<std::string , std::string>> findDocumentsMap(std::string DBName , std::string CollName);


private:
    
    DataBaseConfigs conf;

    //std::unique_ptr<mongocxx::instance> instance_;
    //std::unique_ptr<mongocxx::pool::entry> entry_private;
    //mongocxx::pool pool_private;
    std::string uri_string;

    std::shared_ptr<mongocxx::pool> pool_private;

    mongocxx::pool& pool_ref(){
        mongocxx::pool& pool_ref = *pool_private;
        return pool_ref;
    }

    //std::unique_ptr<mongocxx::instance> instance_private;

};

#endif // MONGODB_H

