//Firmware KCC - Ver4.5 deviceX-V3 25APR24  //19APR24 //11APR24  //06APR24   //5April24  //20MAR24
//Change NTP server to ntp.medtronic.com in NTPClient.h library (2nd May 2025)
#include "globalVar.h"
#include "mlxLogic.h"
#include "visualCueAndRI.h"
#include "hardwareSetup.h"
#include "hubFunction.h"
#include "sensors.h"
#include "inputCommand.h"
#include "mqttpub.h"
#include "roomIndicatorFunction.h"

int f_wifi_MQTT = 0;
unsigned long  start_time_reconnect = 0;
unsigned long  time_duration_wifi_reconnect;
//unsigned long  cycle_time;
//unsigned long  start_cycle_time;

//mqtt sub {mac_address}_commnad callback function
void callback(char* topic, byte* payload, unsigned int length) {
  String usb_comdata = "";
  for (int i = 0; i < length; i++) {
    usb_comdata = usb_comdata + (char)payload[i];
  }
  usb_comdata.trim();
  Serial.println("From topic: " + String(topic) + "\r\nmessage get: " + String(usb_comdata));

  //sub tob space id
  if (strcmp(topic, (char*)subtopicCallBack.c_str()) == 0) {
    //Serial.println("Message of backend space id: " + String(usb_comdata));
    mrsData = usb_comdata;
  }

  //sub to mac address command
  if (strcmp(topic, (char*)mqttSubTopicCallBack.c_str()) == 0) {
    if (usb_comdata.indexOf("conf_data:") > -1) {
      spaceID = usb_comdata.substring(usb_comdata.indexOf("conf_data") + 10);
      client.setServer(mqttServer, mqttPort);
      client.setCallback(callback);
      Serial.println("Connecting to MQTT...by Callback");
      if (client.connect((char*)mqtt_client.c_str(), mqttUser, mqttPassword )) {
        Serial.println("WiFi MQTT connected...by Callback");
        f_wifi_MQTT = 2;
        client.subscribe((char*)mqttSubTopic.c_str());
        subtopic = "$share/prospace_device_x_group/" + String(spaceID) + "_status";
        client.subscribe((char*)subtopic.c_str());
        spaceIdFlag = 1;
      }
      else {
        Serial.println("MQTT connection fail by Callback");
        f_wifi_MQTT = 0;
      }
    }
        else{
      inputCommand(usb_comdata);
    }
  }
}

void setup() {
  //Hardware Initialization for MCU
  hardware_init();	//by hardwareSetup.h

  //Read EEPROM Wifi Info
  if (!EEPROM.begin(1024)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...EEPROM");
    delay(1000);	//in setup()
    ESP.restart();  //by EEPROM
  }

  String checkFirstBoot = EEPROM.readString(448);
  if (checkFirstBoot != "factoryDefalut") {
    // will only run once after production to avoid memory error
    Serial.println("set to factory defalut setting....");
    EEPROM.writeString(0, "SET UP SSID");
    EEPROM.writeString(32, "PASSWORD");
    EEPROM.writeString(64, resetautomlxpar);
    EEPROM.writeString(96, resetEnvMode);
    EEPROM.writeString(128, resetHpMode);
    EEPROM.writeString(160, resetMrsMode);
    EEPROM.writeString(192, resetHubMode);
    EEPROM.writeString(224, resetMlxproc);
    EEPROM.writeString(256, resetMlxraw);
    EEPROM.writeString(288, resetPirpar);
    EEPROM.writeString(320, resetPirMode);
    EEPROM.writeString(352, resetWifiMode); //no longer in use
    EEPROM.writeString(384, resetHpSensor);
    EEPROM.writeString(416, resetBleMode);
    EEPROM.writeString(448, "factoryDefalut");
    EEPROM.writeString(480, resetAutoTemp);
    EEPROM.writeString(512, resetmanualmlxpar);
    EEPROM.commit();
  }

  Serial.println("user setting....");
  wifi_ssid = EEPROM.readString(0);
  wifi_password = EEPROM.readString(32);
  envMode = EEPROM.readString(96);
  hpMode = EEPROM.readString(128);
  mrsMode = EEPROM.readString(160);
  hubMode = EEPROM.readString(192);
  mlxproc = EEPROM.readString(224);
  mlxraw = EEPROM.readString(256);
  pirpar = EEPROM.readString(288);
  pirMode = EEPROM.readString(320);
  wifiMode = EEPROM.readString(352);  //no longer in use
  hpSensor = EEPROM.readString(384);
  BleMode = EEPROM.readString(416);
  autoTemp = EEPROM.readString(480);

  Serial.println("ssid: " + wifi_ssid);
  Serial.println("current setup....");
    if(autoTemp == "mlxminauto-on"){
    mlxpar = EEPROM.readString(64);
    splitMlxpar(mlxpar);
  }
    else{
    mlxpar = EEPROM.readString(512);
    Serial.println("debug at mlxminauto-off: " + mlxpar);
    splitMlxpar(mlxpar);
  }
  Serial.println("envMode: " + envMode);
  Serial.println("hpMode: " + hpMode);
  Serial.println("hpSensor: " + hpSensor);
  Serial.println("mrsMode: " + mrsMode);
  Serial.println("pirMode: " + pirMode);
  Serial.println("hubMode: " + hubMode);
  Serial.println("mlxproc: " + mlxproc);
  Serial.println("mlxraw: " + mlxraw);
  Serial.println("pirpar: " + pirpar);
  Serial.println("BleMode: " + BleMode);
  Serial.println("autoTemp: " + autoTemp);
  // asign pirCounter
  pirCounter = pirpar.toInt();
  wifi_ssid.toCharArray(WIFI_SSID, wifi_ssid.length() + 1);
  wifi_password.toCharArray(WIFI_PASSWORD, wifi_password.length() + 1);
  wifi_status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);	//wifi_status1 by WiFi.begin in setup(

  mqtt_client = WiFi.macAddress();
  Serial.println("mac_address: " + String(mqtt_client));

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  // timeClient.setTimeOffset(28800);
  //Serial.println("Sensor Check Start ");
  delay(100);  //delay(1000);	//in setup()
  //init sensor
  sensorCheck();	//by setup()
  Serial.println("Firmware KCC - Ver4.5 deviceX-V3 25APR24");     //md on 06APR24 //Added timer0_fire 240320

}

void loop() {

  //Serial.println("loop counter: " + String(loop_Counter));
  //loop_Counter ++;

  if (f_s1) {
    s1_Counter = 10 ;
    f_s1 = 0;
    loop_Counter =0;
    //Serial.println("1s Timer");
    
    if (LEDg_Togle){
    digitalWrite(LEDG, LEDg_STATE);    //Toggle LED
    LEDg_STATE = !LEDg_STATE;         //toggle logic
    }
     
  }
  //start_cycle_time = millis();

  //Sync Time & Set Timestamp
    if(syncTime){
    syncTimeStamp();
  }
    else{
    tm_epoch = now();
  }

  timerWrite(timer2, 0); //reset timer (feed watchdog)

  //Reset Modem if not done before
  if (rst_modem_flag) {
    rst_modem();
    rst_modem_flag = 0;
  }

  //print out instruction for command
  if (pow_modem_flag == 0) { //Still Powering Up
    commandInstruction();
  }

  // coolterms input
    if (Serial.available() > 0){
    String usb_comdata = Serial.readString();
    usb_comdata.trim();
    inputCommand(usb_comdata);
  }
  //2s timer fire
  /*     if (timer3_fire) {//2s timer3 fire
          //Serial.println("2s timer Trigger");
          timer3_fire = 0;
  */
  if (f_s2) {//2s timer
/*
    Serial.println("2s Timer"); //debug
    Serial.print("Existing ssid: [ "); //debug
    Serial.print(WIFI_SSID); //debug
    Serial.print(" ] "); //debug
*/
    //s2_Counter = 20;
    //////////////////S2 wifi //////////////////////
    //  uint8_t status = WiFi.waitForConnectResult();

    // check for wifi connection
    //mk on 25Apr24 wifi_status = WiFi.status();  // via s2 wifi_status2 by WiFi.status();
    if (wifi_status != WL_CONNECTED) {
      //Serial.println("WIFI No Conneted ! via 2s"); //debug
      
      wifi_Check_Counter++;
	  
      syncTime = 1; //Time need to sync
	  
      digitalWrite(LEDR, 0);      //R LED//redVisualCue();
	  
      LEDg_Togle = 0;
      digitalWrite(LEDG, 1);    //G LED
      //Serial.println("WiFi 2s Check Counter: " + String(wifi_Check_Counter));
    }
    else {
      wifiCounter = 0;
      wifi_Check_Counter = 0;
      //Serial.println("WIFI Conneted via 2s");

      // digitalWrite(LEDG, LOW);
      digitalWrite(LEDR, 1);   //redVisualCue();   //R LED
      
      LEDg_Togle = 1;
      
      //detect people in the room, MLX or PIR
      if (numberOfPeople > 0) {
      LEDg_Togle = 0;
      digitalWrite(LEDG, 0);    //G LED
		  //greenVisualCue();	//G LED
      }
      else {
      LEDg_Togle = 1;
      //offVisualCue();		//off LED

      }

    }

    // debug for wifi status
      //Serial.println("WiFi Status: " + String(wifi_status));
      //Serial.println("WiFi MQTT: " + String(f_wifi_MQTT));   
    //Serial.println("WiFi 2s Check Counter: " + String(wifi_Check_Counter));
    //////////////////S2 wifi Above//////////////////////
    
    //////////////////S2 Tasks Below //////////////////////
    //get room indicator data
    roomIndicator();

    if (mrsMode == "mrs-on") {
      if (mrsData == "81") {
        greenRI();	//NO part
      }
      else if (mrsData == "82") {
        yellowRI();	//NO part
      }
      else if (mrsData == "83") {
        redRI();	//NO part
      }
    }
    else {
      if (hpSensor == "hpmlx") {
        if (numberOfPeople > 0) {
          redRI();	//NO part
        }
        else {
          greenRI();	//NO part
        }
      }
      else if (hpSensor == "hppir") {
        if (pirCountDown < pirCounter) {
          redRI();	//NO part
        }
        else {
          greenRI();	//NO part
        }
      }
            else{
        Serial.println("please check hpSensor mode");
      }
    }
    //////////////////S2 Tasks Over /////////////////////

    s2_Counter = 20;
    f_s2 = 0;
    loop_Counter = 0;    //in (f_s2)
    //delay(300);  //in (f_s2) debug delay
  }

  //5s timer fire
  /*    if (timer0_fire) {//5s timer fire
         //Serial.println("5s timer Trigger");
         timer0_fire = 0; */
  if (f_s5) {//5s timer fire
    //Serial.println("5s Timer");

    //////////////////S5 Tasks Below /////////////////////
    //get MLX data if mlx in working
    if (mlxStatus == "\"mlx\": 1,") {
      MLX("raw");
    }
    if (client.connected()) {
      // need to get data from mlx again??
      fivesec_mqtt_publish();	//in 5s timer0 fire
    }

    if (mlxraw == "mlxraw-on") {
      mlx_raw_data_counter ++;
      //Serial.println("mlx_raw_data_counter: "+ String(mlx_raw_data_counter));    //debug display on 24Apr24
      // mlxraw off after 10 min of sending data
      if (mlx_raw_data_counter >= 120) {
        mlx_raw_data_counter = 0;
        mlxraw = "mlxraw-off";
        //Serial.print("mlxraw change to off: ");
        //Serial.println(mlxraw);
      }
    }
   //////////////////S5 Tasks Over /////////////////////    
    s5_Counter = 50;
    f_s5 = 0;
    loop_Counter = 0;
    //delay(300);  //in (f_s5) debug delay
  }

  //10s timer fire
  /*     if (timer1_fire) {//10s timer fire
          //Serial.println("10s timer Trigger");
          if(spaceIdFlag == 0){
              spaceIdCounter ++;
          }
          timer1_fire = 0; */
  if (f_s10) {//10s timer fire
    //Serial.println("10s Timer");

    //////////////////S10 Tasks Below /////////////////////

    if (spaceIdFlag == 0) {
      spaceIdCounter ++;
    }

    pirCountDown = pirCountDown + 10;
    get_environment_data(); //in sensor.h
    //get MLX data if mlx in working
    if (mlxStatus == "\"mlx\": 1,") {
      MLX("process");
    }
    if (client.connected()) {
      tensec_mqtt_publish();	//in mqttpub.h in 10s timer1 fire
    }
    
    s10_Counter = 100;
    f_s10 = 0;
    loop_Counter = 0;
    //delay(300);  //in (f_s10) debug delay
  }
    //////////////////S10 Tasks Below /////////////////////
    
  pirSensor();  //U500
  hubFunction();
  BleHubFunction();

    wifi_status = WiFi.status();  //Move in loop 25Apr24
    if (!client.connected()) {
        //Serial.println("client.connected");
        if (wifi_status == WL_CONNECTED){
            wifi_mqtt_reconnect();
        }
        else{
        wifi_reconnect();
      
      if (wifiCounter > 100) {
        Serial.println("ESP reboot....wifi_reconnect");
        delay(1000);  //if (wifiCounter > 100) debug delay
        wifiCounter = 0;
        ESP.restart();  //by wifiCounter > 100
      } 
     }
    }

  client.loop(); //Wifi MQTT
  /////////
  //cycle_time = (millis() - start_cycle_time); //add on 240322
  //Serial.print("LOOP cycle time: ");
  //Serial.println(cycle_time);
  /////////////
}

void syncTimeStamp() {
  if (!timeClient.update()) {
    Serial.println("NTP Time Force Update");
    timeClient.forceUpdate();
  }
  else {
    syncTime = 0;
    setTime(timeClient.getEpochTime());
    tm_epoch = now();
    Serial.print("syncTime:");
    Serial.println(tm_epoch);
  }
}

void rst_modem() {	//for U3 ccs8111
  Serial.println("Modem Hard Reset");
  digitalWrite(MODEM_RST_PIN, LOW);		//MODEM_RST
  pow_modem_flag = 0;
  delay(1000);	//in rst_modem()
  //pinMode(MODEM_RST_PIN, OUTPUT);	//mv to hardwareSetup.h Apr24
  digitalWrite(MODEM_RST_PIN, HIGH);
}

void commandInstruction() {	//for U3 ccs8111
  Serial.println("Do you want to enter new WiFi profile and Setting?");
  Serial.println("Please key in ssid, password, or commands. Thereafter type \"commit\" and press \"Enter\" to connect to this Wifi Profile and Setting.");
  pow_modem_flag = 1;
}
void wifi_reconnect() {
  if (f_wifi_reconnect) {
    if (wifi_status != WL_CONNECTED){
        WiFi.disconnect();
        //Serial.print("Reconnect attempt: ");
        //Serial.println(String(wifiCounter));
        //Serial.println(wifiCounter);
        Serial.print("\nConnecting to existing WiFi profile: SSID: ");
        Serial.print("[ ");
        Serial.print(WIFI_SSID); Serial.print(" | ");
        //Serial.print(WIFI_PASSWORD);
        Serial.print("xxxxxx");
        Serial.println(" ]");
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        wifi_status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);	//in wifi_reconnect(
		
        wifiCounter++;
        Serial.println("Reconnect attempt: " + String(wifiCounter)); //by wifi_reconnect
    }
        f_wifi_reconnect = 0; //add on 240419
        wifi_reconnetion_ct = 50 ;	
  }    //Added timer0_fire 240320
}

void wifi_mqtt_reconnect() {
 if (wifi_status == WL_CONNECTED) {   //add in loop 25Apr24
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  Serial.println("Connecting to MQTT...by wifi_mqtt");
  if (client.connect((char*)mqtt_client.c_str(), mqttUser, mqttPassword )) {
    Serial.println("WiFi MQTT connected...by wifi_mqtt");
    f_wifi_MQTT = 1;
    mqttSubTopic = "$share/prospace_device_x_group/" + String(mqtt_client) + "_command";
    mqttSubTopicCallBack = String(mqtt_client) + "_command";
    client.subscribe((char*)mqttSubTopic.c_str());
    if (mqtt_flag == 1) {
      client.publish("request_configuration_data", (char*)mqtt_client.c_str());
      mqtt_flag = 0;
    }
  }
  else {
    Serial.println("MQTT connection fail by wifi_mqtt");
    f_wifi_MQTT = 0;
  }
}
}
