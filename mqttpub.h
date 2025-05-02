void twosec_mqtt_publish() {
  //send mlx processed data
  Serial.println("no in use");
}

void fivesec_mqtt_publish() {
  //send mlx raw data to mqtt
  if (mlxraw == "mlxraw-on") {
    mlx_raw_data = "{\"mac_address\": \"" + String(mqtt_client) + "\"" +  "," + "\"raw_data\": \"" + mlx_raw_data.substring(0, mlx_raw_data.length() - 1) + "\"}";
    Serial.println("mlx_raw_data: " + mlx_raw_data);
    client.publish("mlx_raw_data", (char*)mlx_raw_data.c_str());
    mlx_raw_data = "";   //add on 25Apr24
    for (int i = 1; i < 25; i++) {
      for (int j = 1; j < 33; j++) {
    Serial.print(image[i][j]);
      }
    Serial.println("");
    }
  }
}

void tensec_mqtt_publish() {
  //digitalWrite(WIFI_LED_PIN, LOW);
  if ((mlxproc != "mlxproc-off") && (mlxStatus == "\"mlx\": 1,")) {
    Serial.println("mlx_processed_data: " + mlx_processed_data);
    client.publish("mlx_processed_data", (char*)mlx_processed_data.c_str());
  }
  if (envMode != "env-off") {
    Serial.println("environment data: " + environment_data );
    client.publish("environment_data", (char*)environment_data.c_str());
  }
  if (hpMode == "hp-on") {
    Serial.println("room_indicator_data: " + room_indicator_data );
    client.publish("room_indicator_data", (char*)room_indicator_data.c_str());
  }
  if (hubMode != "hub-off") {
    sensordata = "{" + String(mqtt_client) + "}" + sensordata;
    Serial.println("sensordata: " + sensordata );
    client.publish("sensordata", (char*)sensordata.c_str());
    sensordata = "";
  }
  if (BleMode != "BLE-OFF"){
    BLEsensordata = "{" + String(mqtt_client) + "}" + BLEsensordata;
    Serial.println("BLEsensordata: " + BLEsensordata );
    client.publish("sensordata", (char*)BLEsensordata.c_str());
    BLEsensordata = "";
  }
  if (pirMode == "pir-on") {
    pir_data = "{" + String(mqtt_client) + "}" + pir_data;
    Serial.println("pir_data: " + pir_data );
    client.publish("pir_data", (char*)pir_data.c_str());
    pir_data = "";
  }
  if ((spaceIdCounter >= 3) && (spaceIdFlag == 0)) {
    client.publish("request_configuration_data", (char*)mqtt_client.c_str());
    spaceIdCounter = 0;
  }
}
