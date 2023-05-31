#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "InfluxClient.h"
using namespace std;

int main() {
    string token = "wjekXomKdcaKiRl7AYUeDTmMGWkHd-I-lwuk3V1F28x5uLSCtaUpspQnEOZXlpnnkQIWAAWB75Fu1U7g9R6iFQ==";
    string ip = "10.111.233.99:80";
    string orgID = "8b1bfc63e1f3a4f8";
    string name = "new_bucket_cpp_6";
    int everySeconds = 604800;
    
    createInfluxClient(token, ip, orgID, name, everySeconds);
    
    

    return 0;
}