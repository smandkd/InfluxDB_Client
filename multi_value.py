import psutil as psutil
from datetime import datetime
from influxdb_client import InfluxDBClient, Point, WriteOptions
from influxdb_client import WriteApi, WriteOptions
from influxdb_client.client.write_api import SYNCHRONOUS
from influxdb_client.client.influxdb_client import InfluxDBClient
import atexit

url = "10.111.233.99:80"
token = "wjekXomKdcaKiRl7AYUeDTmMGWkHd-I-lwuk3V1F28x5uLSCtaUpspQnEOZXlpnnkQIWAAWB75Fu1U7g9R6iFQ=="
org="8b1bfc63e1f3a4f8"
bucket="my-cpu-usage"

client = InfluxDBClient(url=url, token=token, org=org )

def send_data(data_point):
    write_api = client.write_api(write_options=SYNCHRONOUS)
    write_api.write(bucket=bucket, record=data_point)

def on_exit(db_client: InfluxDBClient, write_api: WriteApi):
    write_api.close();
    db_client.close();

while True:
    # CPU 사용률 및 패킷 수 가져오기
    cpu_usage = psutil.cpu_percent(interval=1)
    packet_count = psutil.net_io_counters().bytes_sent

    # 데이터 포인트 생성
    cpu_data_point = Point("cpu_usage") \
        .tag("hostname", "sang") \
        .field("usage", cpu_usage) \
        .time(datetime.utcnow())

    packet_data_point = Point("packet_count") \
        .tag("hostname", "sang") \
        .field("count", packet_count) \
        .time(datetime.utcnow())

    # 데이터 전송
    send_data(cpu_data_point)
    send_data(packet_data_point)

atexit.register(on_exit, _db_client, _write_api)
input()