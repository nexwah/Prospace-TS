//function to get pir value
void pirSensor() {
  int val = digitalRead(PIR);
  //Serial.println(val);
  //Serial.println("pir detected: " + String(isDetected));
  if (val == HIGH) {
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      pirCountDown = 0;
      pir_data = pir_data + "{" + tm_epoch + ",1}";
      pirState = HIGH;
    }
  }
  else {

    if (pirState == HIGH) {
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}

//function to get environment data
void get_environment_data() {
  Serial.println("environment sensor get data");
  environment_data = "";
  temperature = htu.readTemperature();
  humidity = htu.readHumidity();
  if(autoTemp == "mlxminauto-on"){
    minTemp = String(temperature + 0.3);
    Serial.println("current room temperature: " + String(temperature));
    Serial.println("auto mode on minTemp: " + minTemp);
  }
  
  /* air */
  if (ccs.available()) {
    //Serial.println("ccs available");
    if (!ccs.readData()) {
      //Serial.println("ccs read");
      co2 = ccs.geteCO2();
      voc = ccs.getTVOC();
      // publish to this topic when re-init sensor
      //$share/nexwah/environment_data_testing
      String testMessage = "testing...";
      client.publish("environment_data_testing", (char*)testMessage.c_str());
    }
    else{
      String testMessage = String(mqtt_client) + ": re-init U3(CCS811) due to cannot read data from the air quality sensor ";
      Serial.println("testMessage: " + testMessage);
      client.publish("environment_data_testing", (char*)testMessage.c_str());
      digitalWrite(ccsReset, LOW);
      delay(20);
      digitalWrite(ccsReset, HIGH);
      delay(50);
      ccs.SWReset();
      delay(100);
      if(!ccs.begin()){
        Serial.println("re-init sensor U3(CCS811)");
      }
    }
  }
  else{
    String testMessage = String(mqtt_client) + ": Start U15(ENS160) due to air quality sensor U3(CCS811) is not available now ";
    Serial.println("testMessage: " + testMessage);
    client.publish("environment_data_testing", (char*)testMessage.c_str());
    digitalWrite(ccsReset, LOW);
    delay(20);
    digitalWrite(ccsReset, HIGH);
    delay(50);
    ccs.SWReset();
    delay(100);
    if(!ccs.begin()){
      Serial.println("re-init U3(CCS811) sensor");
    }
    Serial.println("Change to U15(ENS160) sensor");
    co2 = ENS160.getECO2();
    voc = ENS160.getTVOC();
  }
  
  if ((co2 == 0) && (voc == 0)){
    String testMessage = String(mqtt_client) + ": re-init due to co2 and voc are 0 ";
    Serial.println("testMessage: " + testMessage);
    client.publish("environment_data_testing", (char*)testMessage.c_str());
    digitalWrite(ccsReset, LOW);
    delay(20);
    digitalWrite(ccsReset, HIGH);
    delay(50);
    ccs.SWReset();
    delay(100);
    if(!ccs.begin()){
      Serial.println("re-init sensor");
    }
  }
  environment_data = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + ", \"co2\": " + String(co2) + ", \"voc\": " + String(voc) + ", \"mac_address\": \"" + String(mqtt_client) + "\"}";
}
