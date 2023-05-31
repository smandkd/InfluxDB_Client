// 되는코드
#include <iostream>
#include <string>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main() {
    CURL* curl = curl_easy_init();

    if(curl) {
        std::string url = "http://10.111.233.99:80/api/v2/write?org=8b1bfc63e1f3a4f8&bucket=2b095df7d1cd2d54&precision=ns";
        std::string data = R"(
            home,room=LivingRoom temp=21.1,hum=35.9,co=0i 1641024000
            home,room=Kitchen temp=21.0,hum=35.9,co=0i 1641024000
            home,room=LivingRoom temp=21.4,hum=35.9,co=0i 1641027600
            home,room=Kitchen temp=23.0,hum=36.2,co=0i 1641027600
            home,room=LivingRoom temp=21.8,hum=36.0,co=0i 1641031200
            home,room=Kitchen temp=22.7,hum=36.1,co=0i 1641031200
            home,room=LivingRoom temp=22.2,hum=36.0,co=0i 1641034800
            home,room=Kitchen temp=22.4,hum=36.0,co=0i 1641034800
            home,room=LivingRoom temp=22.2,hum=35.9,co=0i 1641038400
            home,room=Kitchen temp=22.5,hum=36.0,co=0i 1641038400
            home,room=LivingRoom temp=22.4,hum=36.0,co=0i 1641042000
            home,room=Kitchen temp=22.8,hum=36.5,co=1i 1641042000
            home,room=LivingRoom temp=22.3,hum=36.1,co=0i 1641045600
            home,room=Kitchen temp=22.8,hum=36.3,co=1i 1641045600
            home,room=LivingRoom temp=22.3,hum=36.1,co=1i 1641049200
            home,room=Kitchen temp=22.7,hum=36.2,co=3i 1641049200
            home,room=LivingRoom temp=22.4,hum=36.0,co=4i 1641052800
            home,room=Kitchen temp=22.4,hum=36.0,co=7i 1641052800
            home,room=LivingRoom temp=22.6,hum=35.9,co=5i 1641056400
            home,room=Kitchen temp=22.7,hum=36.0,co=9i 1641056400
            home,room=LivingRoom temp=22.8,hum=36.2,co=9i 1641060000
            home,room=Kitchen temp=23.3,hum=36.9,co=18i 1641060000
            home,room=LivingRoom temp=22.5,hum=36.3,co=14i 1641063600
            home,room=Kitchen temp=23.1,hum=36.6,co=22i 1641063600
            home,room=LivingRoom temp=22.2,hum=36.4,co=17i 1641067200
            home,room=Kitchen temp=22.7,hum=36.5,co=26i 1641067200
        )";

        std::string token = "wjekXomKdcaKiRl7AYUeDTmMGWkHd-I-lwuk3V1F28x5uLSCtaUpspQnEOZXlpnnkQIWAAWB75Fu1U7g9R6iFQ==";

        struct curl_slist* headers = NULL;
        std::string authHeader = "Authorization: Token " + token;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: text/plain; charset=utf-8");
        headers = curl_slist_append(headers, "Accept: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        std::string response;
        std::string verbose;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, &verbose);
        
        CURLcode res = curl_easy_perform(curl);
        if(res == CURLE_OK) {
            std::cout << "Response : " << response << std::endl;
            std::cout << verbose << std::endl;
        }
        else {
            std::cout << "Failed to insert data. Error:" << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return 0;
}
