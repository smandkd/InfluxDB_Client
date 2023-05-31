#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include "InfluxClient.h"
using namespace std;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

string createLineProtocol() {
    vector<string> measurements = {
        "home",
        "home",
        "home"
    };
    
    vector<vector<string>> tags = {
        {"room", "LivingRoom"},
        {"room", "Kitchen"},
        {"room", "LivingRoom"}
    };

    vector<vector<string>> fields = {
        {"temp", "21.1", "hum", "35.9", "co", "0i"},
        {"temp", "21.0", "hum", "35.9", "co", "0i"},
        {"temp", "21.4", "hum", "35.9", "co", "0i"}
    };

    vector<int> times = {
        1641024000,
        1641024000,
        1641027600
    };

    stringstream ss;

    for (size_t i = 0; i < measurements.size(); ++i) {
        ss << measurements[i];

        // Add tags
        for (size_t j = 0; j < tags[i].size(); j += 2) {
            ss << "," << tags[i][j] << "=" << tags[i][j + 1];
        }

        // Add fields
        ss << " ";
        for (size_t j = 0; j < fields[i].size(); j += 2) {
            ss << fields[i][j] << "=" << fields[i][j + 1];
            if (j < fields[i].size() - 2) {
                ss << ",";
            }
        }

        // Add time
        ss << " " << times[i] << "\n";
    }

    return ss.str();
}


int main() {
    string token = "wjekXomKdcaKiRl7AYUeDTmMGWkHd-I-lwuk3V1F28x5uLSCtaUpspQnEOZXlpnnkQIWAAWB75Fu1U7g9R6iFQ==";
    string ip = "10.111.233.99:80";
    string orgID = "8b1bfc63e1f3a4f8";
    string name = "new_bucket_cpp_7";
    int everySeconds = 604800;
    
    createInfluxClient(token, ip, orgID, name, everySeconds);
    string data = createLineProtocol();

    cout << data << endl;

    // struct curl_slist* headers = NULL;
    // std::string authHeader = "Authorization: Token " + token;
    // headers = curl_slist_append(headers, authHeader.c_str());
    // headers = curl_slist_append(headers, "Content-Type: text/plain; charset=utf-8");
    // headers = curl_slist_append(headers, "Accept: application/json");

    // curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // curl_easy_setopt(curl, CURLOPT_POST, 1L);
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    // std::string response;
    // std::string verbose;
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, &verbose);
    
    // CURLcode res = curl_easy_perform(curl);

    // if(res == CURLE_OK) {
    //     std::cout << "Response : " << response << std::endl;
    //     std::cout << verbose << std::endl;
    // }
    // else {
    //     std::cout << "Failed to insert data. Error:" << curl_easy_strerror(res) << std::endl;
    // }

    // curl_slist_free_all(headers);
    // curl_easy_cleanup(curl);

    return 0;
}