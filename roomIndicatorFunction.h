//function to get room indicator data
void roomIndicator() {
  if (hpSensor == "hpmlx") {
    if (thresholdCounter < (768 * threshold.toInt())) {
      //Serial.println("room indicator by mlx");
      //Serial.println("thresholdCounter: " + String(thresholdCounter));
      //Serial.println("threshold set: " + String(768 * threshold.toInt()));
      if (numberOfPeople > 1) {
        room_indicator_data = "{\"status\": 1, \"mac_address\": \"" + String(mqtt_client) + "\"}";
      }
      else {
        room_indicator_data = "{\"status\": 0, \"mac_address\": \"" + String(mqtt_client) + "\"}";
      }
    }
    else {
      //Serial.println("room indicator by pir");
      if (pirCountDown < pirCounter) {
        room_indicator_data = "{\"status\": 1, \"mac_address\": \"" + String(mqtt_client) + "\"}";
      }
      else {
        room_indicator_data = "{\"status\": 0, \"mac_address\": \"" + String(mqtt_client) + "\"}";
      }
    }
  }
  else if (hpSensor == "hppir") {
    //Serial.println("room indicator by pir");
    //Serial.println("in room indicator");
    //Serial.println("pirCountDown: " + String(pirCountDown)+ "pirCounter: " + String(pirCounter));
    if (pirCountDown < pirCounter) {
      room_indicator_data = "{\"status\": 1, \"mac_address\": \"" + String(mqtt_client) + "\"}";
    }
    else {
      room_indicator_data = "{\"status\": 0, \"mac_address\": \"" + String(mqtt_client) + "\"}";
    }
  }
  else {
    Serial.println("roomIndicatore Error please check the mode");
  }
}
