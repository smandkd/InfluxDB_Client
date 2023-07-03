# InfluxDB_Client
This is InfluxDB Client code with python, c++ that upload 5G network metrics. 
InfluxDB + Grafana

1) influxd 명령어 입력
2) http://localhost:8086 접속하면 UI 확인 가능

home,room=Living\ Room temp=21.1,hum=35.9,co=0i 1641024000

measurement: home
	tags
		room: Living Room or Kitchen	
	fields
		temp: temperature in °C (float)
		hum: percent humidity (float)
		co: carbon monoxide in parts per million (integer)		
	timestamp: Unix timestamp in second precision

name : sang
url : http://localhost:8086 
org “

influx setup
username : sang
password : qksemtl12
organization name : home
bucket name : something
retention period : infinite

First try Token
o5TPfb1hMm4KBzJq1eyJ8X2VHWXdwIg21O4Cwai2UplrnW8yHEVl57tU53vjnBU1Xfe1PvkQmV3FD6S4porf2g==

wget https://dl.influxdata.com/influxdb/releases/influxdb2-2.7.0-amd64.deb
sudo dpkg –i influxdb2-2.7.0-amd64.deb
sudo service influxdb start
influxd version
sudo service influxdb status
influxd

influx setup
influx auth create -> error

Line protocol element parsing
- measurement: Everything before the first unescaped comma before the first whitespace.
- tag set: Key-value pairs between the first unescaped comma and the first unescaped whitespace.
- field set: Key-value pairs between the first and second unescaped whitespaces.
- timestamp: Integer value after the second unescaped whitespace.
Lines are separated by the newline character (\n). Line protocol is whitespace sensitive.

influx write \
  --bucket get-started \
  --precision s "
 home,room=Living\ Room temp=21.1,hum=35.9,co=0i 1641024000
 home,room=Kitchen temp=21.0,hum=35.9,co=0i 1641024000
 home,room=Living\ Room temp=21.4,hum=35.9,co=0i 1641027600
 home,room=Kitchen temp=23.0,hum=36.2,co=0i 1641027600
 home,room=Living\ Room temp=21.8,hum=36.0,co=0i 1641031200
 home,room=Kitchen temp=22.7,hum=36.1,co=0i 1641031200
 home,room=Living\ Room temp=22.2,hum=36.0,co=0i 1641034800
 home,room=Kitchen temp=22.4,hum=36.0,co=0i 1641034800
 home,room=Living\ Room temp=22.2,hum=35.9,co=0i 1641038400
 home,room=Kitchen temp=22.5,hum=36.0,co=0i 1641038400
 home,room=Living\ Room temp=22.4,hum=36.0,co=0i 1641042000
 home,room=Kitchen temp=22.8,hum=36.5,co=1i 1641042000
 home,room=Living\ Room temp=22.3,hum=36.1,co=0i 1641045600
 home,room=Kitchen temp=22.8,hum=36.3,co=1i 1641045600
 home,room=Living\ Room temp=22.3,hum=36.1,co=1i 1641049200
 home,room=Kitchen temp=22.7,hum=36.2,co=3i 1641049200
 home,room=Living\ Room temp=22.4,hum=36.0,co=4i 1641052800
 home,room=Kitchen temp=22.4,hum=36.0,co=7i 1641052800
 home,room=Living\ Room temp=22.6,hum=35.9,co=5i 1641056400
 home,room=Kitchen temp=22.7,hum=36.0,co=9i 1641056400
 home,room=Living\ Room temp=22.8,hum=36.2,co=9i 1641060000
 home,room=Kitchen temp=23.3,hum=36.9,co=18i 1641060000
 home,room=Living\ Room temp=22.5,hum=36.3,co=14i 1641063600
 home,room=Kitchen temp=23.1,hum=36.6,co=22i 1641063600
 home,room=Living\ Room temp=22.2,hum=36.4,co=17i 1641067200
 home,room=Kitchen temp=22.7,hum=36.5,co=26i 1641067200
 "
influx query '
 from(bucket: "get-started")
     |> range(start: 2022-01-01T08:00:00Z, stop: 2022-01-01T20:00:01Z)
     |> filter(fn: (r) => r._measurement == "home")
     |> filter(fn: (r) => r._field== "co" or r._field == "hum" or r._field == "temp")
 '

influx v1 shell

InfluxQL Shell 2.5.0
Connected to InfluxDB OSS v2.7.0

> SELECT co,hum,temp,room FROM "get-started".autogen.home WHERE time >= '2022-01-01T08:00:00Z' AND time <= '2022-01-01T20:00:00Z’




> exit

Grafana 

-설치
sudo apt-get install -y adduser libfontonfig1
wget https://dl.grafana.com/oss/release/grafana_8.0.6_amd64.deb
sudo dpkg –i grafana_8.0.6_amd64.deb

-실행
sudo systemctl daemon-reload
sudo systemctl start grafana-server
sudo systemctl status grafana-server
sudo systemctl enable grafana-server

http://localhost:3000
username : admin
passoword : amdin
비밀번호 잊을 시 
grafana-cli admin reset-admin-password admin

Your first Flux query from Grafana
Open Grafana Explorer located in the left menu of Grafana GUI as the Compass icon.

Type the following simple query on line 1 of the Explorer:

buckets()Copy
Hit the Run Query button located at the top right corner. The result is the table listing 3 buckets.

The buckets _monitoring and _tasks are internal InfluxDB buckets. The bucket is called my-bucket, which was created with the influx setup command run earlier.

fluxdb query
from(bucket: "get-started")
     |> range(start: 2022-01-01T08:00:00Z, stop: 2022-01-01T20:00:01Z)
     |> filter(fn: (r) => r._measurement == "home")



