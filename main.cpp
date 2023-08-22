#include "mongodb-tools/mongodb.h"
#include <chrono>
#include <ctime>
#include <iomanip>

    std::tm parseDateTime(const std::string& datetimeStr) {
    std::istringstream ss(datetimeStr);
    std::tm dt = {};
    ss >> std::get_time(&dt, "%Y:%m:%d %H:%M:%S");
    return dt;
    }

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

    int fps = 30;

    std::cout << "hi\n";
    std::vector<std::string> list = MyObj.ListDBs();

    for (std::string item : list){
        std::cout << item << std::endl;
    }

    std::vector<std::map<std::string , std::string>> map = MyObj.findDocumentsMap("aid" , "profiles");
    std::vector<std::map<std::string , std::string>> map0 = MyObj0.findDocumentsMap("aid" , "profiles");

    MyObj.~mongodb();
    std::string input = "2023:08:21 18:22:30";
    std::string correct_format = "2023-05-29T12:39:59.344+00:00";
    // std::tm tm;
    // std::istringstream ss(input);
    // ss >> std::get_time(&tm, "%Y:%m:%d %H:%M:%S");
    // bsoncxx::types::b_date date(std::chrono::milliseconds{std::mktime(&tm)});

    // std::tm tm; 
    // std::istringstream(input) >> std::get_time(&tm, "%Y:%m:%d %H:%M:%S");

    // // Convert tm to formatted string  
    // std::string date;
    // std::ostringstream oss;
    // oss << std::put_time(&tm, "%F %T");
    // date = oss.str();

    // bsoncxx::types::b_date date{1692625950};

    // bsoncxx::types::b_date date{std::chrono::system_clock::now()};

    // // Convert to tm
    // std::time_t t = date;
    // std::tm* tm = std::gmtime(&t); 

    // // Print formatted tm 
    // std::cout << std::put_time(tm, "%F %T") << std::endl;


    std::string input = "2023:08:21 18:22:30";
    // std::tm time = {};
    // std::istringstream ss(input);
    // ss >> std::get_time(&time, "%Y:%m:%d %H:%M:%S");

    // if (ss.fail()) {
    //     std::cout << "Failed to parse the input string." << std::endl;
    //     return 1;
    // }

    // std::ostringstream os;
    // os << std::put_time(&time, "%Y-%m-%dT%H:%M:%S");
    // std::string isoDate = os.str();

    // std::tm date = {};
    // std::istringstream ss(input);
    // ss >> std::get_time(&date, "%Y:%m:%d %H:%M:%S");
    // if (ss.fail()) {
    //     std::cout << "Failed to parse date string." << std::endl;
    //     return 1; // or handle the error appropriately
    // }

    // std::time_t time = std::mktime(&date);
    // bsoncxx::types::b_date bsonDate{std::chrono::system_clock::from_time_t(time)};
    // std::tm tmTime = boost::posix_time::to_tm(boost::posix_time::time_from_string(input));

    //std::cout << MyObj0.ConvertToDate("\" stream_fps\":\"30") << std::endl;

    // std::tm date_parsed = parseDateTime(input);
    // std::time_t time_test = std::mktime(&date_parsed);
    // std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time_test);
    // std::time_t convert = std::chrono::system_clock::to_time_t(tp);
    // std::tm* localtime = std::localtime(&convert);
    // std::ostringstream oss;
    // oss << std::put_time(localtime , "%Y-%m-%dT%H:%M:%S");
    // std::string isodatetime = oss.str();

    // bsoncxx::types::b_date date(tp);


    bsoncxx::types::b_date date_test{std::chrono::system_clock::now()}; // Replace with your b_date object

    bsoncxx::builder::basic::document doc{};
    doc.append(bsoncxx::builder::basic::kvp("date", date_test));

    bsoncxx::document::view view = doc.view();
    std::string json = bsoncxx::to_json(view);

    std::tm time = {};
    // std::istringstream ss(std::chrono::system_clock::now());
    // ss >> std::get_time(&time , "%Y:%m:%d %H:%M:%S");
    // std::time_t timestamp = std::mktime(&time);

    std::ostringstream oss;
    oss << std::put_time(&time, "%Y-%m-%dT%H:%M:%S");
    std::string timestampString = oss.str() + ".344+00:00";

    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(std::mktime(&time));
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    std::string millisecondsString = std::to_string(milliseconds);

    std::string numb = "1692691397514";
    std::string value = "ISODate" + correct_format;
    std::cout << "{\"date\":" + MyObj.ConvertToDate(input) + "}" << std::endl;
    std::cout << MyObj0.ConvertToDate(input) << std::endl;
    if(MyObj.InsertDocument("hello" , "test1" , "{\"date\":" + MyObj.ConvertToDate(input) + "}"));

    std::cout << "bye" << std::endl;

    return 0;
}
