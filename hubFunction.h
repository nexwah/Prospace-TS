//BLE hub function
void BleHubFunction() {
  // put your main code here, to run repeatedly:
  while (MySerial2.available() > 0) {
    bc = MySerial2.read();
    //Serial.println(bc);

    for (int i = 0; i < 15; i++) {
      Blerf_msg[i] = Blerf_msg[i + 1];
    }
    Blerf_msg[15] = bc;
    if ((Blerf_msg[0] == 0x54) && (Blerf_msg[1] == 0x7B) && (Blerf_msg[13] == 0x7D) && (Blerf_msg[14] == 0x44)) { //
      BleRec rec;
      rec.rf_id1 = Blerf_msg[2];
      rec.rf_id2 = Blerf_msg[3];
      rec.rf_id3 = Blerf_msg[4];
      rec.rf_id4 = Blerf_msg[5];
      rec.rf_id5 = Blerf_msg[6];
      rec.rf_id6 = Blerf_msg[7];
      rec.rf_id7 = Blerf_msg[8];
      rec.rf_id8 = Blerf_msg[9];
      rec.rf_tag1 = Blerf_msg[10];
      rec.rf_tag2 = Blerf_msg[11];
      bq.push(&rec);
    }
  }
  while (bq.isEmpty() == 0) {
    //Serial.println("q empty");
    BleRec rec;
    bq.pop(&rec);
    sprintf(BLEpayload, "{%s,%c%c%c%c%c%c%c%c,%c%c}", tm_epoch, rec.rf_id1, rec.rf_id2, rec.rf_id3, rec.rf_id4, rec.rf_id5, rec.rf_id6, rec.rf_id7, rec.rf_id8, rec.rf_tag1, rec.rf_tag2);
    //Serial.print("BLE payload: ");
    //Serial.println(BLEpayload);
    BLEsensordata = BLEsensordata + String(BLEpayload);
  }
}

//hub funciton
void hubFunction() {
  while (MySerial1.available() > 0) {
    c = MySerial1.read(); //From RF
    for (int i = 0; i < 11; i++) {
      rf_msg[i] = rf_msg[i + 1];
    }
    rf_msg[11] = c;
    if ((rf_msg[0] == 0x52) && (rf_msg[1] == 0x7B) && (rf_msg[9] == 0x7D) && (rf_msg[10] == 0x44)) { //
      Rec rec;
      rec.rf_id1 = rf_msg[2];
      rec.rf_id2 = rf_msg[3];
      rec.rf_id3 = rf_msg[4];
      rec.rf_id4 = rf_msg[5];
      rec.rf_tag = rf_msg[8];
      q.push(&rec);
    }
  }
  while (q.isEmpty() == 0) {
    //Serial.println("q empty");
    Rec rec;
    q.pop(&rec);
    sprintf(payload, "{%s,%02X%02X%02X%02X,%02X}", tm_epoch, rec.rf_id1, rec.rf_id2, rec.rf_id3, rec.rf_id4, rec.rf_tag);
    sensordata = sensordata + String(payload);
  }
}
