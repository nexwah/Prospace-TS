int LEDr_STATE=LOW;// 0 on 08Apr
int LEDg_STATE=LOW;// 5 on 08Apr
int LEDb_STATE=LOW;// 4 on 08Apr

int LEDr_Togle=0;// on 08Apr 
int LEDg_Togle=0;// on 08Apr 
int LEDb_Togle=0;// on 08Apr 
 
void IRAM_ATTR onTimer0() {
  timer0_fire = 1;
  timer0_Counter ++;
  
//add on Apr24
	s_1_Counter --;
	s_5_Counter --;
	s1_Counter --;
	s2_Counter --;
	s5_Counter --;
	s10_Counter --;
	wifi_reconnetion_ct --;
	
	if (!s_5_Counter){
	LEDb_STATE= !LEDb_STATE;  				//toggle LEDB logic
	//digitalWrite(LEDB, LEDb_STATE);    //Toggle LEDB

	s_5_Counter = 5 ;
	}

  if (!s1_Counter){
    f_s1 = 1;
    s1_Counter = 10 ;
  }

 if (!s2_Counter){
    f_s2 = 1;
    s2_Counter = 20 ;
   }
   
 if (!s5_Counter){
    f_s5 = 1;
    s5_Counter = 50 ;
  }
  
  if (!s10_Counter){
    f_s10 = 1;
     s10_Counter = 100 ;
  }
  
  if (!wifi_reconnetion_ct ){
    f_wifi_reconnect = 1;
    wifi_reconnetion_ct = 50 ;
  }
//add on Apr24    
 

} 
  
void IRAM_ATTR onTimer1() {	//NA for new y24
  timer1_fire = 1;
  timer1_Counter ++;
}

void IRAM_ATTR resetModule() {
  Serial.print("reboot\n");
  esp_restart();
}

void IRAM_ATTR onTimer3() {	//NA for new y24
  timer3_fire = 1;
  timer3_Counter ++;
}

//for visual cue
void pca932Setup() {	//NA on Nexwah board
  //software reset
  Wire.beginTransmission(0x03);
  Wire.write(0xA5);
  Wire.endTransmission();

  //set to mode 1
  Wire.beginTransmission(0x62);
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission();

  //set to mode 2
  Wire.beginTransmission(0x62);
  Wire.write(0x01);
  Wire.write(0x1A);
  Wire.endTransmission();

  //set to PWM0
  Wire.beginTransmission(0x62);
  Wire.write(0x02);
  Wire.write(0x80);
  Wire.endTransmission();

  //set to PWM1
  Wire.beginTransmission(0x62);
  Wire.write(0x03);
  Wire.write(0x80);
  Wire.endTransmission();

  //set to PWM2
  Wire.beginTransmission(0x62);
  Wire.write(0x04);
  Wire.write(0x80);
  Wire.endTransmission();

  //set to PWM3
  Wire.beginTransmission(0x62);
  Wire.write(0x05);
  Wire.write(0x80);
  Wire.endTransmission();
}

void hardware_init() {
  Serial.begin(115200);
  MySerial1.begin(115200, SERIAL_8N1, 13, 14); //RX, TX, SubG
  MySerial2.begin(115200, SERIAL_8N1, 16, 17); //RX, TX, SubG
  Wire.begin();
  Wire.setClock(400000);

  pinMode(ccsWake,OUTPUT);
  pinMode(ccsReset,OUTPUT); //U3 CSS811
  delay(100);
  pinMode(ccsReset,HIGH);
  digitalWrite(ccsWake, LOW); //Turn on U3 CSS811

//NA on Nexwah board
  pinMode(red, OUTPUT);
  digitalWrite(red, HIGH);

  pinMode(green, OUTPUT);
  digitalWrite(green, HIGH);

  pinMode(blue, OUTPUT);
  digitalWrite(blue, HIGH);
//NA on Nexwah board


  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, HIGH);

  pinMode(LEDG, OUTPUT);
  digitalWrite(LEDG, HIGH);

  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, HIGH);
  
  pinMode(MODEM_RST_PIN, OUTPUT);	//mv from rst_modem() Apr24
  
  pinMode(PIR, INPUT);	//for PIR U500 NS612 and BT U200 nRF52811

  //pca932Setup();//NA on Nexwah board

   
  //timer_setup(); //Setup a timer

  /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
  timer0 = timerBegin(0, 80, true);	//with 0.5S heart beat by LEDb add for new y24
  timer1 = timerBegin(1, 80, true);	//NA for new y24
  timer2 = timerBegin(2, 80, true);
  timer3 = timerBegin(3, 80, true);  //5mins heart beat	//NA for new y24

  /* Attach onTimer function to our timer */
  timerAttachInterrupt(timer0, &onTimer0, true);	//with 0.5S heart beat 
  timerAttachInterrupt(timer1, &onTimer1, true);	//NA for new y24
  timerAttachInterrupt(timer2, &resetModule, true);  //attach callback
  timerAttachInterrupt(timer3, &onTimer3, true);	//NA for new y24

  /* Set alarm to call onTimer function every second 1 tick is 1us
    => 1 second is 1000000us */
  /* Repeat the alarm (third parameter) */
  timerAlarmWrite(timer0, 100000, true); //0.1S 100000	//mk Apr24 5s 5000000
  timerAlarmWrite(timer1, 10000000, true); //10s	//NA for new y24
  timerAlarmWrite(timer2, wdtTimeout * 1000, false); //set time in us
  timerAlarmWrite(timer3, 2000000, true); //2s	//NA for new y24

  /* Start an alarm */
  timerAlarmEnable(timer0);
  //Serial.println("start timer0");
  timerAlarmEnable(timer1);	//NA for new y24
  //Serial.println("start timer1");
  timerAlarmEnable(timer2);                          //enable interrupt
  //Serial.println("start timer2 watchdog");
  timerAlarmEnable(timer3);	//NA for new y24
  //Serial.println("start timer3");array
}

//Returns true if the MLX90640 is detected on the I2C bus
boolean isConnected()		//for IR Array U8 MLX90640ESF
{
  Wire.beginTransmission((uint8_t)MLX90640_address);
  if (Wire.endTransmission() != 0)
    return (false); //Sensor did not ACK
  return (true);
}

//sensor setup and check stauts
void sensorCheck() {	
  if (isConnected() == false) {	//for IR Array U8 MLX90640ESF
    Serial.println("mlx - offline");
    mlxStatus = "\"mlx\": 0,";
  }
  else {
    Serial.println("mlx - normal");
    mlxStatus = "\"mlx\": 1,";
  }
  int status;
  uint16_t eeMLX90640[832];
  status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  if (status != 0) {
    Serial.println("Failed to load system parameters");
    mlxStatus = "\"mlx\": 0,";
  }
  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0) {
    Serial.println("Parameter extraction failed");
    mlxStatus = "\"mlx\": 0,";
  }
  //check for mlx data

  for (byte x = 0 ; x < 2 ; x++) //Read both subpages
  {
    uint16_t mlx90640Frame[834];
    int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
    if (status < 0)
    {
      Serial.print("GetFrame Error: ");
      Serial.println(status);
    }

    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    float emissivity = 0.95;

    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
  }
  if ((mlx90640To[300]>0) && (mlx90640To[300]<100)){
    mlxStatus = "\"mlx\": 1,";
  }
  else{
    mlxStatus = "\"mlx\": 0,"; 
  }

  if(!htu.begin()) {	//???U6 HTU21D
    Serial.println("Temperature - offline");
    Serial.println("Humidity - offline");
    tempStatus = "\"temp\": 0,";
    humStatus = "\"humidity\": 0,";
  }
  else {
    Serial.println("Temperature - normal");
    Serial.println("Humidity - normal");
    tempStatus = "\"temp\": 1,";
    humStatus = "\"humidity\": 1,";
  }
  
  if(!ccs.begin()){
    Serial.println("Sensor_U3_CCS811_voc - offline");
    Serial.println("Sensor_U3_CCS811_CO2 - offline");
    vocStatus = "\"voc\": 0,";
    co2Status = "\"co2\": 0,";
  }
  else{
    Serial.println("Sensor_U3_CCS811_voc - normal");
    Serial.println("Sensor_U3_CCS811_CO2 - normal");
    vocStatus = "\"voc\": 1,";
    co2Status = "\"co2\": 1,";
  }

  if(NO_ERR != ENS160.begin()){
    Serial.println("Sensor_U15_ENS160_voc - offline");
    Serial.println("Sensor_U15_ENS160_CO2 - offline");
    vocStatus = "\"voc\": 0,";
    co2Status = "\"co2\": 0,";
  }
  else{
    ENS160.setPWRMode(ENS160_STANDARD_MODE);
    ENS160.setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);
    Serial.println("Sensor_U15_ENS160_voc - normal");
    Serial.println("Sensor_U15_ENS160_CO2 - normal");
    vocStatus = "\"voc\": 1,";
    co2Status = "\"co2\": 1,";
  }
}
