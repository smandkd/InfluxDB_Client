#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <chrono>
#include "InfluxClient.h"
#include "Point.h"
using namespace std;

// InfluxDB Client with C++
// JSON 형식의 문자열을 받아 nlohmann 라이브러리를 사용해서 JSON으로 변환하여 InfluxDB에 적재하는 코드

// class InfluxDBClient : 이 클래스는 InfluxDB API를 만들어주는 클래스이다. 
// [InfluxDBClient] 생성자와 [createBucket], [WriteCallback], [writeAPI] 함수가 있다. 
// [InfluxDBClient] : IP, Token, Orgid, Bucket, Retentionpolicy 를 초기화해준다.
// [createBucket] : 이 함수를 통해 버킷을 생성한다.
// [WriteCallback] : InfluxDB에 Write API 요청을 보낼 때 Callback 형식으로 Response를 출력해준다.
// [writeAPI] : Point 클래스의 data를 API에 담아 InfluxDB로 Request 해주는 함수이다. 

// class Point : InfluxDB API에 담아서 보낼 data 를 Measurement, Tag, Field, Timestamp 의 LineProtocol에 맞춰 문자열을 만들어주는 클래스이다. 

// InfluxDB 로컬 : http://10.111.233.99/orgs/8b1bfc63e1f3a4f8/data-explorer?bucket=new_bucket_cpp_8
// ID : admin
// PW : tdVXuaknO1GbO3FNstVsFZDyPI3MdCBY
// 조희 쿼리 : 
// from( bucket : "new_bucket_cpp_8")
// |> range(start : 0)
// |> filter(fn: (r) => r._measurement == "ue0")
// |> filter(fn: (r) => r.type == "DL")
// |> filter(fn: (r) => r._field == "brate" or r._field == "cqi" or r._field == "mcs")

int main() {
    string ip = "10.111.233.99:80"; // Kubernetes 상의 InfluxDB Container의 IP + Port 
    string token = "wjekXomKdcaKiRl7AYUeDTmMGWkHd-I-lwuk3V1F28x5uLSCtaUpspQnEOZXlpnnkQIWAAWB75Fu1U7g9R6iFQ==";
    string orgID = "8b1bfc63e1f3a4f8"; // An organization is a workspace for a group of users. All dashboards, tasks, buckets, members, etc., belongs to an organization. 
    string bucket = "new_bucket_cpp_8";
    int retentionpolicy = 604800;
    
    InfluxDBClient influx = InfluxDBClient(ip, token, orgID, bucket, retentionpolicy); // InfluxDB API를 위한 기본적인 정보와 함께 생성자 생성
    influx.createBucket(); // new_bucket_cpp_7 버킷 생성 

    string temp_data = R"({"DL":{"brate":126.98412698412699,"cqi":15,"mcs":0,"nok":0,"nok_per":0,"ok":2,"ri":0},"UL":{"brate":50784.58049886621,"bsr":0,"mcs":3.0,"nok":0,"nok_per":0,"ok":15,"phr":40,"pucch":3.090944766998291,"pusch":6.309884548187256},"pci":1,"rat":"lte","rnti":70})";
    nlohmann::json json_data = nlohmann::json::parse(temp_data);
    //std::cout << json_data["DL"]["brate"] << std::endl;  

    auto timestamp = chrono::system_clock::now(); // 지금 시간

    Point point("ue0");
    point.tag("type", "DL")
         .field("brate", json_data["DL"]["brate"])
         .field("cqi", json_data["DL"]["cqi"])
         .field("mcs", json_data["DL"]["mcs"])
         .field("nok", json_data["DL"]["nok"])
         .field("nok_per", json_data["DL"]["nok_per"])
         .field("ok", json_data["DL"]["ok"])
         .field("ri", json_data["DL"]["ri"])
         .time(timestamp);
    
    Point point_1("ue0");
    point_1.tag("type", "UL")
         .field("brate", json_data["UL"]["brate"])
         .field("cqi", json_data["UL"]["bsr"])
         .field("mcs", json_data["UL"]["mcs"])
         .field("nok", json_data["UL"]["nok"])
         .field("nok_per", json_data["UL"]["nok_per"])
         .field("ok", json_data["UL"]["ok"])
         .field("ri", json_data["UL"]["phr"])
         .field("pucch", json_data["UL"]["pucch"])
         .time(timestamp);

    string data = point.toLineProtocol();
    string data_1 = point_1.toLineProtocol();
    cout << data << endl;
    cout << data_1 << endl;

    influx.writeAPI(data);
    influx.writeAPI(data_1);

    return 0;
}