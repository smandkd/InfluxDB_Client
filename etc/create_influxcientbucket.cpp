// 되는거 
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
using namespace std;

class InfluxDBClient {
public:
    
}

void createInfluxClient(string &token_, string &ip_, const string& orgID_, const string& buck_name,int everySeconds) {
    string t = "Authorization: Token ";
    string authHeader = t.append(token_);
    
    string url_prefix = "http://";
    string url_suffix = "/api/v2/buckets";
    string url = url_prefix.append(ip_).append(url_suffix);
    
    nlohmann::json jsonData_1 = {
        {"orgID", orgID_},
        {"name", buck_name},
        {"retentionRules", nlohmann::json::array({{
            {"type", "expire"},
            {"everySeconds", everySeconds}
        }})} 
    };

    CURL* curl;
    CURLcode res;  

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if( curl ) {
        struct curl_slist *headers = NULL;
        string jsonData = jsonData_1.dump();
        string contentTypeHeader = "Content-type: application/json";

        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, contentTypeHeader.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            cerr << "Failed to create bucket: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }    
    curl_global_cleanup();

}


int main() {
    string token = "wjekXomKdcaKiRl7AYUeDTmMGWkHd-I-lwuk3V1F28x5uLSCtaUpspQnEOZXlpnnkQIWAAWB75Fu1U7g9R6iFQ==";
    string ip = "10.111.233.99:80";
    string orgID = "8b1bfc63e1f3a4f8";
    string name = "new_bucket_cpp_5";
    int everySeconds = 604800;
    
    createInfluxClient(token, ip, orgID, name, everySeconds);
    
    return 0;
}
