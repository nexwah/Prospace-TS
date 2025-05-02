void inputCommand(String usb_comdata){
  if (usb_comdata.indexOf("ssid:") > -1) {
    wifi_ssid = usb_comdata.substring(usb_comdata.indexOf("ssid") + 5);
    Serial.println("wifi ssid: " + wifi_ssid );
    //delay(300); //debug delay add for test delay(1000);
  }
  else if (usb_comdata.indexOf("password:") > -1) {
    wifi_password = usb_comdata.substring(usb_comdata.indexOf("password") + 9);
    Serial.println("wifi password: " + wifi_password );
    //delay(300); //debug delay add for test delay(1000);
  }
  else if (usb_comdata.indexOf("mlxpar:") > -1) {
    autoTemp = EEPROM.readString(480);
    mlxpar = usb_comdata.substring(usb_comdata.indexOf("mlxpar") + 7);
    Serial.println(autoTemp + " mlxpar: " + mlxpar );
    splitMlxpar(mlxpar);
        if(autoTemp == "mlxminauto-on"){
      EEPROM.writeString(64, mlxpar);
      EEPROM.commit();
    }
        else{
      EEPROM.writeString(512, mlxpar);
      EEPROM.commit();
    }
  }
  else if ((usb_comdata == "env-on") || (usb_comdata == "env-off")) {
    envMode = usb_comdata;
    Serial.println("envMode: " + envMode );
    EEPROM.writeString(96, envMode);
    EEPROM.commit();
  }
  else if ((usb_comdata == "hp-on") || (usb_comdata == "hp-off")) {
    hpMode = usb_comdata;
    Serial.println("hpMode: " + hpMode );
    EEPROM.writeString(128, hpMode);
    EEPROM.commit();
  }
  else if (usb_comdata.indexOf("hppir") > -1) {
    hpSensor = usb_comdata;
    Serial.println("hpSensor: " + hpSensor );
    EEPROM.writeString(384, hpSensor);
    EEPROM.commit();
  }
  else if (usb_comdata.indexOf("hpmlx") > -1) {
    hpSensor = usb_comdata;
    Serial.println("hpSensor: " + hpSensor );
    EEPROM.writeString(384, hpSensor);
    EEPROM.commit();
  }
  else if ((usb_comdata == "mrs-on") || (usb_comdata == "mrs-off")) {
    mrsMode = usb_comdata;
    Serial.println("mrsMode: " + mrsMode );
    EEPROM.writeString(160, mrsMode);
    EEPROM.commit();
  }
  else if ((usb_comdata == "hub-on") || (usb_comdata == "hub-off")) {
    hubMode = usb_comdata;
    Serial.println("hubMode: " + hubMode );
    EEPROM.writeString(192, hubMode);
    EEPROM.commit();
  }
  else if ((usb_comdata == "mlxproc-on") || (usb_comdata == "mlxproc-off")) {
    mlxproc = usb_comdata;
    Serial.println("mlxproc: " + mlxproc );
    EEPROM.writeString(224, mlxproc);
    EEPROM.commit();
  }
  else if ((usb_comdata == "mlxraw-on") || (usb_comdata == "mlxraw-off")) {
    mlxraw = usb_comdata;
    Serial.println("mlxraw: " + mlxraw );
    EEPROM.writeString(256, mlxraw);
    EEPROM.commit();
  }
  else if (usb_comdata.indexOf("pirpar:") > -1) {
    pirpar = usb_comdata.substring(usb_comdata.indexOf("pirpar") + 7);
    Serial.println("pirpar: " + pirpar );
    pirCounter = pirpar.toInt();
        if(pirCounter <=600){
      EEPROM.writeString(288, pirpar);
      EEPROM.commit();
    }
        else{
      Serial.println("pir max value is 600");
      // check topic with mak and qiyue
      client.publish("status_check", "pir max value is 600");
    }
  }
  else if ((usb_comdata == "pir-on") || (usb_comdata == "pir-off")) {
    pirMode = usb_comdata;
    Serial.println("pirMode: " + pirMode );
    EEPROM.writeString(320, pirMode);
    EEPROM.commit();
  }
  else if ((usb_comdata == "ble-on") || (usb_comdata == "ble-off")) {
    BleMode = usb_comdata;
    Serial.println("BleMode: " + BleMode );
    EEPROM.writeString(416, BleMode);
    EEPROM.commit();
  }
  else if ((usb_comdata == "mlxminauto-on") || (usb_comdata == "mlxminauto-off")) {
    autoTemp = usb_comdata;
    Serial.println("autoTemp: " + autoTemp );
    EEPROM.writeString(480, autoTemp);
    EEPROM.commit();
        if(autoTemp == "mlxminauto-on"){
      mlxpar = EEPROM.readString(64);
      splitMlxpar(mlxpar);
    }
        else{
      mlxpar = EEPROM.readString(512);
      splitMlxpar(mlxpar);
    }
  }
  else if (usb_comdata == "dxreset") {
    Serial.println("reseting the device X back to default");
    delay(500);
    EEPROM.writeString(64, resetautomlxpar);
    EEPROM.writeString(96, resetEnvMode);
    EEPROM.writeString(128, resetHpMode);
    EEPROM.writeString(160, resetMrsMode);
    EEPROM.writeString(192, resetHubMode);
    EEPROM.writeString(224, resetMlxproc);
    EEPROM.writeString(256, resetMlxraw);
    EEPROM.writeString(288, resetPirpar);
    EEPROM.writeString(320, resetPirMode);
    EEPROM.writeString(352, resetWifiMode);
    EEPROM.writeString(384, resetHpSensor);
    EEPROM.writeString(416, resetBleMode);
    EEPROM.writeString(480, resetAutoTemp);
    EEPROM.writeString(512, resetmanualmlxpar);
    EEPROM.commit();
    automlxpar = EEPROM.readString(64);
    envMode = EEPROM.readString(96);
    hpMode = EEPROM.readString(128);
    mrsMode = EEPROM.readString(160);
    hubMode = EEPROM.readString(192);
    mlxproc = EEPROM.readString(224);
    mlxraw = EEPROM.readString(256);
    pirpar = EEPROM.readString(288);
    pirMode = EEPROM.readString(320);
    wifiMode = EEPROM.readString(352);
    hpSensor = EEPROM.readString(384);
    BleMode = EEPROM.readString(416);
    autoTemp = EEPROM.readString(480);
    manualmlxpar = EEPROM.readString(512);
        if(autoTemp == "mlxminauto-on"){
      mlxpar = EEPROM.readString(64);
    }
        else{
      mlxpar = EEPROM.readString(512);
    }
    if ((automlxpar == resetautomlxpar) && (manualmlxpar == resetmanualmlxpar) && (envMode == resetEnvMode) && (hpMode == resetHpMode) && (mrsMode == resetMrsMode) && (hubMode == resetHubMode) && (mlxproc == resetMlxproc) && (mlxraw == resetMlxraw) && (pirpar == resetPirpar) && (pirMode == resetPirMode) && (wifiMode == resetWifiMode) && (hpSensor == resetHpSensor) && (BleMode == resetBleMode)) {
      String Status = "{\"mac_address\": \"" + String(mqtt_client) + "\"," + "\"status\": \"success\"}";
      client.publish("status_check", (char*)Status.c_str());
      Serial.println("dxreset success");
    }
    else {
      String Status = "{\"mac_address\": \"" + String(mqtt_client) + "\"," + "\"status\": \"fail\"}";
      client.publish("status_check", (char*)Status.c_str());
      Serial.println("dxreset fail");
    }
    //ESP.restart();
  }
  else if (usb_comdata == "wifireboot") {
    Serial.println("reboot now...via remote command");//mak on 11Apr24
    delay(1000);     //2000
    ESP.restart();
  }
  else if (usb_comdata == "dxsensorstatus") {
    sensorCheck();
    if (client.connected()) {
      Serial.println("MQTT connection - normal");
      mqttStatus = "\"mqtt\": 1";
    }
    else {
      Serial.println("MQTT connection - offline");
      mqttStatus = "\"mqtt\": 0";
    }
    String sensorStatus = "{\"type\":\"sensor_status\",\"data\":{" + mlxStatus + tempStatus + humStatus + vocStatus + co2Status + mqttStatus + "},\"mac_address\":\"" + String(mqtt_client) + "\"}";
    client.publish("status_check", (char*)sensorStatus.c_str());
  }
  else if (usb_comdata == "dxstatus") {
    autoTemp = EEPROM.readString(480);
        if(autoTemp == "mlxminauto-on"){
      mlxpar = EEPROM.readString(64);
    }
        else{
      mlxpar = EEPROM.readString(512);
    }
    Serial.println("wifiMode: " + wifiMode);
    Serial.println(envMode);
    Serial.println(hpSensor);
    Serial.println(hpMode);
    Serial.println(mrsMode);
    Serial.println(pirMode);
    Serial.println("pirpar:" + pirpar);
    Serial.println("mlxpar: " + mlxpar);
    Serial.println(mlxproc);
    Serial.println(mlxraw);
    Serial.println(BleMode);
    Serial.println(autoTemp);
    splitMlxpar(mlxpar);
    String deviceXstatus = "{\"type\":\"device_status\",\"data\":{\"general\":[\"" + envMode + "\",\"" + mlxproc + "\",\"" + mlxraw + "\",\"" + BleMode + "\",\"" + autoTemp + "\"],\"pirpar\":" + pirpar + ",\"mlxpar\":{\"min\":" + minTemp + ",\"max\":" + maxTemp + ",\"pixel\":" + pixel + ",\"threshold\":" + threshold + "},\"mac_address\":\"" + String(mqtt_client) + "\"}}";
        Serial.println("dxstatus: "+ deviceXstatus );
    client.publish("status_check", (char*)deviceXstatus.c_str());
    delay(500);
    //String deviceXstatus = "{\"type\":\"device_status\",\"data\":{\"general\":[\""+envMode+"\",\""+hpSensor+"\",\""+hpMode+"\",\""+mrsMode+"\",\""+mlxproc+"\",\""+mlxraw+"\",\""+hubMode+"\"],\"mlxpar\":{\"min\":"+minTemp+",\"max\":"+maxTemp+",\"pixel\":"+pixel+",\"threshold\":"+threshold+"}}}";
    //Serial.println(deviceXstatus);
  }
  else if (usb_comdata.indexOf("commit") > -1) {
    EEPROM.writeString(0, wifi_ssid);
    EEPROM.writeString(32, wifi_password);
    EEPROM.commit();
    Serial.println("setting configured!" );
    wifi_ssid.toCharArray(WIFI_SSID, wifi_ssid.length() + 1);
    wifi_password.toCharArray(WIFI_PASSWORD, wifi_password.length() + 1);
    WiFi.disconnect();  //add on 24Apr24
    //delay(300); //debug delay add for test delay(1000);
  }
  else {
    Serial.println("you have key in wrong command....");
    //delay(300); //debug delay add for test delay(1000);
  }
}
