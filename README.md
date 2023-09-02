# ESP32新北宿舍电量监测 BUAA_XinBei_PowerMonitor
### 于北航新北15公寓得到充分测试
新北电量查询方式为HTTP POST，故可使用ESP32向目标服务器POST学号信息来获取你所在宿舍的剩余电量
#### 使用说明:
```
const char* ssid = "........";              //这里填你的WIFI SSID
const char* password =  "........";      //这里填你的WIFI password
const char* stucode = "........";       //这里填你的Student ID
```
