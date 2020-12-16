#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


const char ssid[]     = "f";         // 使用时请修改为当前你的 wifi 名称 -- Please use your own wifi ssid
const char password[] = "12345678";     //  使用时请修改为当前你的 wifi 密码 -- Please use your own wifi password
const char OneNetServer[] = "api.heclouds.com";
const char APIKEY[] = "XXXXXXXXXXXXX";    
int32_t DeviceId = 643814844;                                                 
char DataStreams[] = "Switch";               
const size_t MAX_CONTENT_SIZE = 1024;               
const unsigned long HTTP_TIMEOUT = 2100;              

int stream;                       

WiFiClient client;
const int tcpPort = 80;

struct UserData
{
  int errno_val;                // 错误返回值 -- Return error code
  char error[32];               // 错误返回信息 -- Return error information
  int recived_val;             // 接收数据值 -- Recived data
  char udate_at[32];            // 最后更新时间及日期 -- Last time for update
};

//
//跳过 HTTP 头，使我们在响应正文的开头 -- Skip HTTP headers so that we are at the beginning of the response's body
//
bool skipResponseHeaders()
{
  char endOfHeaders[] = "\r\n\r\n";
  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);
  return ok;
}
//
//从HTTP服务器响应中读取正文 -- Read the body of the response from the HTTP server
//
void readReponseContent(char* content, size_t maxSize)
{
  //  size_t length = client.peekBytes(content, maxSize);
  size_t length = client.readBytes(content, maxSize);
  delay(20);
  content[length] = 0;
}
//
// 解析数据存储到传入的结构体中 -- Save data to userData struct
//
bool parseUserData_test(char* content, struct UserData* userData)
{
  // 根据我们需要解析的数据来计算JSON缓冲区最佳大小 -- Compute optimal size of the JSON buffer according to what we need to parse.
  // 如果你使用StaticJsonBuffer时才需要 -- This is only required if you use StaticJsonBuffer.
  const size_t BUFFER_SIZE = 1024;
  // 在堆栈上分配一个临时内存池 -- Allocate a temporary memory pool on the stack
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  // 如果堆栈的内存池太大，使用 DynamicJsonBuffer jsonBuffer 代替
  // --If the memory pool is too big for the stack, use this instead:
  //  --DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(content);
  if (!root.success())
  {
    return false;
  }
  // 复制需要的信息到userData结构体中 -- Here were copy the strings we're interested in
  userData->errno_val = root["errno"];
  strcpy(userData->error, root["error"]);
  // 成功返回0 -- Number 0 represents successful
  if ( userData->errno_val == 0 )
  {
    userData->recived_val = root["data"]["datastreams"][0]["datapoints"][0]["value"];
    strcpy(userData->udate_at, root["data"]["datastreams"][0]["datapoints"][0]["at"]);
  }
  return true;
}
//
// 读取数据 -- Read data
//
int readData(int dId, char dataStream[])
{
  // 创建发送请求的URL -- We now create a URI for the request
  String url = "/devices/";
  url += String(dId);
  url += "/datapoints?datastream_id=";
  url += dataStream;

  // 创建发送指令 -- We now combine the request to the server
  String send_data = String("GET ") + url + " HTTP/1.1\r\n" +
                     "api-key:" + APIKEY + "\r\n" +
                     "Host:" + OneNetServer + "\r\n" +
                     "Connection: close\r\n\r\n";
  // 发送指令 -- This will send the request to server
  client.print(send_data);
  // 调试模式串口打印发送的指令 -- The request will be printed if we choose the DEBUG mode
  unsigned long timeout = millis();
  while (client.available() == 0)
  {
    if (millis() - timeout > 2000)
    {
      client.stop();
      break;
    }
  }

  if (skipResponseHeaders())
  {
    char response[MAX_CONTENT_SIZE];
    // 从服务器读取信息后解析 -- We now parse the information after we recived the data
    readReponseContent(response, sizeof(response));
    UserData userData_LEDstatus;
    if (parseUserData_test(response, &userData_LEDstatus))
    {
      return userData_LEDstatus.recived_val;
    }
  }
}

void setup()
{
  WiFi.mode(WIFI_AP_STA);               //设置工作模式 -- set work mode:  WIFI_AP /WIFI_STA /WIFI_AP_STA
  Serial.begin(115200);
  delay(10);

  // 连接到wifi -- We start by connecting to a wifi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void loop()
{
  //建立连接并判断 -- Connecting to server
  if (!client.connect(OneNetServer, tcpPort))
  {
    //Serial.println("connection failed");
    return;
  }
  //从云端获取值并存于stream1 -- get data from server
  stream = readData(DeviceId, DataStreams);
   if(stream == 1) GPIO_OUTPUT_SET(GPIO_ID_PIN(2), 1);
  else GPIO_OUTPUT_SET(GPIO_ID_PIN(2), 0);
  Serial.println(stream);

}
