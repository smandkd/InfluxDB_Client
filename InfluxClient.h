using namespace std;

class InfluxDBClient 
{
    string ip_;
    string token_;
    string orgid_;
    string bucket_;
    int retentionpolicy_;

private: 
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t totalSize = size * nmemb;
        output->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
    std::string url;
    std::string token;
    std::string orgID;

public : 
    InfluxDBClient(const string &a, const string &b, const string &c, const string &d, const int e)
    {   
        ip_ = a;
        token_ = b;
        orgid_ = c;
        bucket_ = d;
        retentionpolicy_ = e;
    }

    void showVariables(string &dummy) {
        // cout << url_ << token_ << orgid_;
        cout << dummy;
    }

    void createBucket() {
        string t = "Authorization: Token ";
        string authHeader = t.append(token_);  

        string url_prefix = "http://";
        string url_suffix = "/api/v2/buckets";
        string url = url_prefix.append(ip_).append(url_suffix);

        nlohmann::json jsonData_1 = {
            {"orgID", orgid_},
            {"name", bucket_},
            {"retentionRules", nlohmann::json::array({{
                {"type", "expire"},
                {"everySeconds", retentionpolicy_}
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
    

    void writeAPI(string &data) {
        CURL* curl = curl_easy_init();
        struct curl_slist* headers = NULL;
        string authHeader = "Authorization: Token " + token_;
        // string url = "http://10.111.233.99:80/api/v2/write?org=8b1bfc63e1f3a4f8&bucket=2b095df7d1cd2d54&precision=ns";

        string url_prefix = "http://";
        string url_org = "/api/v2/write?org=";
        string url_bucket = "&bucket=";
        string url_precision = "&precision=ns";
        string url = url_prefix.append(ip_).append(url_org).append(orgid_).append(url_bucket).append(bucket_).append(url_precision);
        showVariables(url);

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
};