#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <PubSubClient.h>
#include "EEPROM.h"
#include "esp_system.h"
#include <cppQueue.h>
#include "Adafruit_HTU21DF.h"
#include "Adafruit_CCS811.h"
#include <HardwareSerial.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#include <DFRobot_ENS160.h>

#define I2C_COMMUNICATION
#define MODEM_RST_PIN 25	//for U3 ccs8111
//#define WIFI_LED_PIN 2
#define ESP_LED_PIN 27

//NA on Nexwah board
//RI color 
#define green 2
#define red 15
#define blue 27
//NA on Nexwah board

//RGB LED (D1)
#define LEDR 0
#define LEDG 5
#define LEDB 4

//PIR
#define PIR 34	//for PIR U500 NS612 and BT U200 nRF52811

// ccs pin
#define ccsWake 12	//for U3 ccs8111
#define ccsReset 25	//for U3 ccs8111

#define TA_SHIFT 8 //Default shift for MLX90640 in open air

//ENS160
#ifdef  I2C_COMMUNICATION
  DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x52);	//#define I2C_COMMUNICATION
#else
  uint8_t csPin = D3;
  DFRobot_ENS160_SPI ENS160(&SPI, csPin);
#endif

// uart for hubs
HardwareSerial MySerial1(1);
HardwareSerial MySerial2(2);

// mqtt obj
WiFiClient espClient;
PubSubClient client(espClient);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//HTU CCS
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
Adafruit_CCS811 ccs;
float temperature = 0;
float humidity = 0;
int voc = 0;
int co2 = 0;


/* create hardware timer */
hw_timer_t * timer0 = NULL;
hw_timer_t * timer1 = NULL;
hw_timer_t * timer2 = NULL; //watchdog
hw_timer_t * timer3 = NULL;

int timer0_fire = 0;
int timer1_fire = 0;
int timer3_fire = 0;
volatile int timer0_Counter;
volatile int timer1_Counter;
volatile int timer3_Counter;

//add on Apr24
int f_s_1  = 0;
int f_s_5  = 0;
int f_s1  = 0;
int f_s2  = 0;
int f_s5  = 0;
int f_s10  = 0;
int f_wifi_reconnect  = 0;

int s_1_Counter = 1;
int s_5_Counter = 5;
int s1_Counter = 10;
int s2_Counter = 20;
int s5_Counter = 50;
int s10_Counter = 100;
int wifi_reconnetion_ct = 50;

int loop_Counter = 0;
int wifi_Check_Counter = 0;
//add on Apr24

const int wdtTimeout = 100000;  //time in ms to trigger the watchdog 100S 

//wifi
char WIFI_SSID[32] = "";
char WIFI_PASSWORD[32] = "";
int wifiCounter = 0;
int wifi_status = WL_IDLE_STATUS;

// mqtt var
//borker endpoint with TLS
//const char* mqttServer = "mqtt-poc.prospace.io";
//borker endpoint without TLS
const char* mqttServer = "mqtt-server-chjvc3bhy2utbxf0dc1zzxj2zxitzw1xltiwmtkwmtaz.prospace.io";
const int mqttPort = 1883;
const char* mqttUser = "cHJvX3B1Yg==";
const char* mqttPassword = "B2kk3wF}BB&4";
String mqtt_client = "prospace";

//MLX sensor
const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640
static float mlx90640To[768];
paramsMLX90640 mlx90640;
String mlx_processed_data = "";
String mlx_raw_data = "";
char image[26][34];
int numberCounter = 0;
String minTemp;
String maxTemp;
String pixel;
String threshold;
int thresholdCounter;
int numberOfPeople = 0;
int numberOfPeopleInCluster = 0;

//sensor status
String pirStatus = "";
String mlxStatus = "";
String tempStatus = "";
String humStatus = "";
String vocStatus = "";
String co2Status = "";
String mqttStatus = "";


//var for device setting
String wifi_ssid;
String wifi_password;
String envMode = "env-on";             //default on
String hpSensor = "hppir";             //default hppir
String hpMode = "hp-off";              //default off
String mrsMode = "mrs-off";            //default off
String pirMode = "pir-off";            //default off
String hubMode = "hub-on";             //default on
String pirpar = "180";                 //max 600
String automlxpar = "24.5,32,5,1";     //default automlxpar:24.5,32,5,1 
String manualmlxpar = "27.5,32,6,1";   //default manualmlxpar:27.5,32,6,1 
String mlxpar = "";        
String mlxproc = "mlxproc-on";         //default on
String mlxraw = "mlxraw-on";           //default on
String wifiMode = "wificonnect-on";    //default on
String BleMode = "ble-on";             //defalut on
String TempRange = "temp-on";          //defalut on
String autoTemp = "mlxminauto-on";           //defalut on

// setting for reset device
String resetEnvMode = "env-on";             //default on
String resetHpSensor = "hppir";             //default hppir
String resetHpMode = "hp-off";              //default off
String resetMrsMode = "mrs-off";            //default off
String resetPirMode = "pir-off";            //default off
String resetHubMode = "hub-on";             //default on
String resetPirpar = "180";                 //max 600
String resetautomlxpar = "24.5,32,5,1";     //default automlxpar:24.5,32,5,1 
String resetmanualmlxpar = "27.5,32,6,1";   //default automlxpar:27.5,32,6,1 
String resetMlxproc = "mlxproc-on";         //default on
String resetMlxraw = "mlxraw-on";          //default on
String resetWifiMode = "wificonnect-on";    //default on
String resetBleMode = "ble-on";             //defalut on
String resetTempRange = "temp-on";          //default on
String resetAutoTemp = "mlxminauto-on";           //default on

//var for hub function
#define  IMPLEMENTATION  FIFO
int c = 0;
char payload[512]; //512
byte rf_msg[11];
typedef struct strRec {
  byte  rf_id1;
  byte  rf_id2;
  byte  rf_id3;
  byte  rf_id4;
  byte  rf_tag;
} Rec;
Rec tab[16] = {
};
Queue q(sizeof(Rec), 16, IMPLEMENTATION); // Instantiate queue

//var for ble hub function
#define  BLEIMPLEMENTATION  FIFO
int bc = 0;
char BLEpayload[512]; //512
byte Blerf_msg[15];
typedef struct BlestrRec {
  char  rf_id1;
  char  rf_id2;
  char  rf_id3;
  char  rf_id4;
  char  rf_id5;
  char  rf_id6;
  char  rf_id7;
  char  rf_id8;
  char  rf_tag1;
  char  rf_tag2;
} BleRec;

BleRec Bletab[16] = {
};
Queue bq(sizeof(BleRec), 16, BLEIMPLEMENTATION); // Instantiate queue


int rst_modem_flag = 1;
int pow_modem_flag = 0;
int mqtt_flag = 1;

String mqtt_payload = "";


String firstboot;
int pirCounter;
int pirCountDown = 0;
String mrsData = "";
String subtopic = "";
String subtopicCallBack = "";
String spaceID = "SPACE_ID";
int pirState = LOW;
int mlx_raw_data_counter = 0;

// for epoch time
int syncTime = 1;
String tm_epoch = "";

//var for data sending to mqtt
String environment_data = "";
String room_indicator_data = "";
String pir_data = "";
String sensordata = "";
String BLEsensordata = "";

String mqttSubTopic = "";
String mqttSubTopicCallBack = "";
int spaceIdFlag = 0 ;
int spaceIdCounter = 0;
