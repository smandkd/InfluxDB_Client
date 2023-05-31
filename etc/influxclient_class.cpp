#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <chrono>
#include "InfluxClient.h"
#include "Point.h"
using namespace std;

// InfluxDB Client with C++
// 단순 string을 적재하는 코드

// class InfluxDBClient : 이 클래스는 InfluxDB API를 만들어주는 클래스이다. 
// [InfluxDBClient] 생성자와 [createBucket], [WriteCallback] 함수와 [writeAPI] 함수가 있다. 
// InfluxDBClient : IP, Token, Orgid, Bucket, Retentionpolicy 를 초기화해준다.
// createBucket : 이 함수를 통해 버킷을 생성한다.
// WriteCallback : InfluxDB에 Write API 요청을 보낼 때 Callback 형식으로 Response를 출력해준다.
// writeAPI : Point 클래스의 data를 API에 담아 InfluxDB로 Request 해주는 함수이다. 

// class Point : InfluxDB API에 담아서 보낼 data 를 Measurement, Tag, Field, Timestamp 의 LineProtocol에 맞춰 문자열을 만들어주는 클래스이다. 

int main() {
    string ip = "10.111.233.99:80"; // Kubernetest 상의 InfluxDB Container의 IP + Port 
    string token = "wjekXomKdcaKiRl7AYUeDTmMGWkHd-I-lwuk3V1F28x5uLSCtaUpspQnEOZXlpnnkQIWAAWB75Fu1U7g9R6iFQ==";
    string orgID = "8b1bfc63e1f3a4f8"; // An organization is a workspace for a group of users. All dashboards, tasks, buckets, members, etc., belongs to an organization. 
    string bucket = "new_bucket_cpp_7";
    int retentionpolicy = 604800;
    
    InfluxDBClient influx = InfluxDBClient(ip, token, orgID, bucket, retentionpolicy); // InfluxDB API를 위한 기본적인 정보와 함께 생성자 생성
    influx.createBucket(); // new_bucket_cpp_7 버킷 생성 

    string cpu_usage = "ul"; // tag
    double packet_sent = 123.435; // field
    double packet_recv = 56.34; // field
    auto timestamp = chrono::system_clock::now(); // 지금 시간

    Point point("sang");
    point.tag("type", "ul")
        .field("pci", packet_sent)
        .field("rnti", packet_recv)
        .time(timestamp);
    
    string data = point.toLineProtocol();
    cout << data << endl;

    influx.writeAPI(data);

    return 0;
}