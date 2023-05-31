// 되는거 
#include <iostream>
#include <curl/curl.h>

int main() {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist *headers = NULL;
        std::string authHeader = "Authorization: Token wjekXomKdcaKiRl7AYUeDTmMGWkHd-I-lwuk3V1F28x5uLSCtaUpspQnEOZXlpnnkQIWAAWB75Fu1U7g9R6iFQ==";
        std::string contentTypeHeader = "Content-type: application/json";
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, contentTypeHeader.c_str());

        std::string url = "http://10.111.233.99:80/api/v2/buckets";

        std::string jsonData = R"({
            "orgID": "8b1bfc63e1f3a4f8",
            "name": "new_bucket_cpp_2",
            "retentionRules": [{"type": "expire", "everySeconds": 604800}]
        })";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "Failed to create bucket: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
