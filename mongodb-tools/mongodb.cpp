#include "mongodb.h"
#include <cstdlib>
#include <memory>
#include <sstream>
#include <iomanip>

std::string pwdDecryption(std::string input) {

    std::string output = input;
    boost::replace_all(output, "%", "%25");
    boost::replace_all(output, "/", "%2F");
    boost::replace_all(output, "?", "%3F");
    boost::replace_all(output, "#", "%23");
    boost::replace_all(output, "[", "%5B");
    boost::replace_all(output, "]", "%5D");
    boost::replace_all(output, "@", "%40");
    boost::replace_all(output, "!", "%21");
    boost::replace_all(output, "$", "%24");
    boost::replace_all(output, "&", "%26");
    boost::replace_all(output, "'", "%27");
    boost::replace_all(output, "(", "%28");
    boost::replace_all(output, ")", "%29");
    boost::replace_all(output, "*", "%2A");
    boost::replace_all(output, "+", "%2B");
    boost::replace_all(output, ",", "%2C");
    boost::replace_all(output, ";", "%3B");
    boost::replace_all(output, "=", "%3D");
    boost::replace_all(output, ":", "%3A");
    boost::replace_all(output, "<", "%3C");
    boost::replace_all(output, ">", "%3E");
    boost::replace_all(output, "\"", "%22");
    boost::replace_all(output, "{", "%7B");
    boost::replace_all(output, "}", "%7D");
    boost::replace_all(output, "|", "%7C");
    boost::replace_all(output, "\\", "%5C");
    boost::replace_all(output, "^", "%5E");
    boost::replace_all(output, "`", "%60");
    boost::replace_all(output, "~", "%7E");

    //std::size_t pos = 0;
    //while (pos != input)
    return output;
}

std::string ConvertTypeValue(bsoncxx::document::element element){

    auto type = element.get_value().type();
    std::string type_str = bsoncxx::to_string(type);
    std::string value;

    mongocxx::pipeline pipeline;

    //std::cout << type_str << std::endl;

    if (type_str == "oid"){
        value = element.get_value().get_oid().value.to_string();
    }

    else if (type_str == "bool"){
        value = (element.get_value().get_bool().value) ? "true" : "false";
    }

    else if (type_str == "int32"){
        value = std::to_string(element.get_value().get_int32().value);
    }

    else if (type_str == "double"){
        value = std::to_string(element.get_value().get_double().value);
    }

    else if (type_str == "array"){
        value = bsoncxx::to_json(element.get_value().get_array().value);
    }

    else if (type_str == "date"){
        auto date = element.get_value().get_date();
        //auto duration = date.to_int64();

        auto tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(date.to_int64()));

        auto unix_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);
        std::tm utc_local = *std::gmtime(&tt);
        utc_local.tm_isdst = -1;
        std::time_t local_time = std::mktime(&utc_local);
        std::tm tm = *std::localtime(&local_time);

        std::stringstream ss;
        ss << std::put_time(&tm , "%Y-%m-%d %H:%M:%S.") << std::setfill('0') << std::setw(3) << (unix_ms % 1000);

        value = ss.str();
    }

    else if (type_str == "null"){
        value = "null";
    }

    else if (type_str == "document"){
        //std::cout << bsoncxx::to_string(element.get_value().type()) << std::endl;
        value = bsoncxx::to_json(element.get_value().get_document().value);
    }

    else{

        value = std::string(element.get_value().get_utf8().value);
    }

    return value;

}

static mongocxx::instance& mongocxx_instance() {
    static mongocxx::instance instance{};
    return instance;
}

mongodb::mongodb(){

    /*try {
        init();
    } catch (const std::exception& e){
        e.what();
        throw;

    }*/

}

/*mongodb::~mongodb(){

    //std::cout << "\033[1;32m" << "Mongodb instance has been closed" << "\033[0m" << std::endl;
    //delete conf;
    //pool_private.reset();
    //mongocxx::instance::~instance();
    //delete pool_private;
    //exit(EXIT_SUCCESS);
    mongocxx_instance().~instance();
}*/

int mongodb::init(DataBaseConfigs Configs){

    if (Configs.Server == "" || Configs.username == "" || Configs.pwd == ""){
        std::cerr << "Authentication failed." << std::endl;
        return EXIT_FAILURE;
    }

    conf = mongodb::DataBaseConfigs(Configs);
    //instance_ = std::make_unique<mongocxx::instance>();
    mongocxx_instance();
    std::cout << "\033[1;32m" << "Mongodb instance initialized successfully" << "\033[0m" << std::endl;

    if (conf.tls == 1){

        std::string uri_str = "mongodb://"+conf.username+":"+pwdDecryption(conf.pwd)+"@"+conf.Server+":"+conf.port+"/?directConnection=true&serverSelectionTimeoutMS=10000&connectTimeoutMS=50000&tls=true&tlsCertificateKeyFile=%2Fetc%2Fssl%2Fmongodb.pem&tlsAllowInvalidCertificates=true";

        //std::cout << uri_str << std::endl;
        uri_string = uri_str;
    }

    if (conf.tls == 0){

        std::string uri_str = "mongodb://"+conf.username+":"+pwdDecryption(conf.pwd)+"@"+conf.Server+":"+conf.port+"/?directConnection=true&serverSelectionTimeoutMS=10000&connectTimeoutMS=50000";

        uri_string = uri_str;
    }

    pool_private = std::make_shared<mongocxx::pool>(mongocxx::uri(uri_string));

    mongodb::isConnectionAlive();

    return 0;
}


void mongodb::isConnectionAlive(){
    mongocxx::pool::entry entry = pool_ref().acquire();

    try{
        entry->database("admin");
        //std::cout << "Connection is Alive" << std::endl;
        entry->reset();

    } catch (const std::exception& e){
        std::cerr << "MongoDB Connection ERROR: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}


bool mongodb::isDBExisting(std::string DBName){

    std::vector<std::string> list = mongodb::ListDBs();
    bool existing = (std::find(list.begin() , list.end() , DBName) != list.end());

    if (existing == true){
        return true;
    }

    else{

        return false;
    }

}

void mongodb::DropDB(std::string DBName){

    if (mongodb::isDBExisting(DBName) == true){

        mongocxx::pool::entry entry = pool_ref().acquire();

        mongocxx::database db = entry->database(DBName);

        std::vector<std::string> colls = db.list_collection_names();

        std::size_t c = 0;
        while (c != colls.size()){
            db.collection(colls[c]).drop();
            c += 1;
        }

        entry->reset();;

        //std::cout << DBName + "DataBase Removed!" << std::endl;
    }

    else{

        std::cerr << DBName + " DataBase doesn't Exist!" << std::endl;

    }
}

void mongodb::CreateDBandCollection(std::string DBName , std::string CollName){

    if (mongodb::isDBExisting(DBName) == true){

        if (mongodb::isCollectionExisting(DBName , CollName) == true){
            std::cerr << CollName + " Collection Exists!" << std::endl;
        }

        else{

            mongocxx::pool::entry entry = pool_ref().acquire();

            mongocxx::database db = entry->database(DBName);
            db.create_collection(CollName);

            entry->reset();
            //std::cout << CollName + " Collection Created!" << std::endl;
        }
    }

    else{

        mongocxx::pool::entry entry = pool_ref().acquire();

        mongocxx::database db = entry->database(DBName);
        db.create_collection(CollName);

        entry->reset();

    }

}

bool mongodb::isCollectionExisting(std::string DBName , std::string CollName){

    if (mongodb::isDBExisting(DBName) == true){

        mongocxx::pool::entry entry = pool_ref().acquire();

        mongocxx::database db = entry->database(DBName);

        if (db.has_collection(CollName) == true){
            //std::cout << CollName + "Collection Exists" << std::endl;
            entry->reset();
            return true;
        }

        else{
            //std::cout << CollName + "Collection doesn't Exist!" << std::endl;
            entry->reset();
            return false;
        }
    }

    else{
        //std::cerr << DBName + " DataBase doesn't Exist!" << std::endl;
        return false;
    }
}

void mongodb::DropCollection(std::string DBName , std::string CollName){

    if (mongodb::isDBExisting(DBName) == true){

        if (mongodb::isCollectionExisting(DBName , CollName) == true){

            mongocxx::pool::entry entry = pool_ref().acquire();

            mongocxx::database db = entry->database(DBName);

            db.collection(CollName).drop();
            entry->reset();
        }

        else{
            std::cerr << CollName + " Collection doesn't Exist!" << std::endl;
        }
    }

    else{
        std::cerr << DBName + " DataBase doesn't Exist!" << std::endl;
    }

}

std::vector<std::string> mongodb::ListDBs(){

    mongocxx::pool::entry entry = pool_ref().acquire();

    std::vector<std::string> list = entry->list_database_names();

    entry->reset();

    return list;
}

std::vector<std::string> mongodb::ListCollections(std::string DBName){

    if (mongodb::isDBExisting(DBName) == true){

        mongocxx::pool::entry entry = pool_ref().acquire();

        mongocxx::database db = entry->database(DBName);

        std::vector<std::string> list = db.list_collection_names();

        entry->reset();

        return list;
    }

    else{
        std::cerr << DBName + " DataBase doesn't Exist!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<std::string> mongodb::findDocuments(std::string DBName , std::string CollName){

    if (mongodb::isDBExisting(DBName) == true){

        if (mongodb::isCollectionExisting(DBName , CollName) == true){

            std::vector<std::string> all_docs;

            mongocxx::pool::entry entry = pool_ref().acquire();

            mongocxx::database db = entry->database(DBName);

            mongocxx::cursor cursor_all = db.collection(CollName).find({});
            for (bsoncxx::document::view doc : cursor_all){
                all_docs.push_back(bsoncxx::to_json(doc));
            }

            entry->reset();

            return all_docs;
        }

        else{
            std::cerr << CollName + " Collection doesn't Exist!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    else{
        std::cerr << DBName + " DataBase doesn't Exist!" << std::endl;
        exit(EXIT_FAILURE);
    }
}


void mongodb::InsertDocument(std::string DBName, std::string CollName, std::string json_input){

    if (mongodb::isDBExisting(DBName) == true){

        if (mongodb::isCollectionExisting(DBName , CollName)){

            bsoncxx::document::value doc_input = bsoncxx::from_json(json_input);

            mongocxx::pool::entry entry = pool_ref().acquire();
            mongocxx::database db = entry->database(DBName);
            db.collection(CollName).insert_one(doc_input.view());

            entry->reset();
        }

        else{
            std::cerr << CollName + " Collection doesn't Exist!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    else{
        std::cerr << DBName + " DataBase doesn't Exist!" << std::endl;
        exit(EXIT_FAILURE);
    }
}


void mongodb::DeleteAllDocuments(std::string DBName , std::string CollName){

    if (mongodb::isDBExisting(DBName) == true){

        if (mongodb::isCollectionExisting(DBName , CollName) == true){

            mongocxx::pool::entry entry = pool_ref().acquire();

            mongocxx::database db = entry->database(DBName);

            db.collection(CollName).drop();

            entry->reset();
        }

        else{
            std::cerr << CollName + " Collection doesn't Exist!" << std::endl;
        }
    }

    else{
        std::cerr << DBName + " DataBase doesn't Exist!" << std::endl;
    }
}

void mongodb::PrintAllDocuments(std::string DBName , std::string CollName){

    if (mongodb::isDBExisting(DBName) == true){

        if (mongodb::isCollectionExisting(DBName , CollName) == true){

            for (std::string doc : mongodb::findDocuments(DBName , CollName)){
                std::cout << doc << std::endl;
            }
        }

        else{
            std::cerr << CollName + " Collection doesn't Exist!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    else{
        std::cerr << DBName + " DataBase doesn't Exit!" << std::endl;
        exit(EXIT_FAILURE);
    }
}



void mongodb::QueryDirectlyInDB(std::string DBName, std::string Conditions){
    //mongocxx::database db = entry_private->database(DBName);
    //mongocxx::database db = (*entry_private)->database(DBName);
    //mongocxx::client& client = **entry_private;
    //mongocxx::database db = client.database(DBName);
    //mongocxx::pool::entry entry = pool_private.acquire();

    mongocxx::pool::entry entry = pool_ref().acquire();

    mongocxx::database db = entry->database(DBName);

    bsoncxx::document::value query = bsoncxx::from_json("R"+Conditions);
    db.run_command(query.view());
}


std::vector<std::map<std::string , std::string>> mongodb::findDocumentsMap(std::string DBName, std::string CollName){
    //mongocxx::pool::entry entry = pool_ref().acquire();

    if (mongodb::isDBExisting(DBName) == true){

        if (mongodb::isCollectionExisting(DBName , CollName) == true){
            std::vector<std::map<std::string , std::string>> result;
            std::vector<std::string> docs = mongodb::findDocuments(DBName , CollName);

            for (std::string doc : docs){
                //std::vector<std::string> key_names;
                std::map<std::string , std::string> mapping;

                bsoncxx::document::value bson_doc = bsoncxx::from_json(doc);
                bsoncxx::document::view bson =  bson_doc.view();
                for (bsoncxx::document::element elem : bson){
                    //key_names.push_back(elem.k.get_utf8().value);

                    std::string key = (elem.key()).to_string();

                    mapping[key] = ConvertTypeValue(elem);

                    std::cout << mapping[key] << std::endl;
                }

                result.push_back(mapping);
                mapping.clear();
            }

            return result;

        }

        else{
            std::cerr << CollName + " Collection doesn't Exist!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    else{
        std::cerr << DBName + " DataBase doesn't Exit!" << std::endl;
        exit(EXIT_FAILURE);
    }
}
