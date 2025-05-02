// function to split mlx parameter
void splitMlxpar(String mlxpar) {
    int splitCounter = 0;
    int j = 0;
    for (int i = 0; i <= mlxpar.length(); i++) {
        if ((mlxpar.charAt(i) == ',') || (i == mlxpar.length())) {
            if (splitCounter == 0) {
                minTemp = mlxpar.substring(j, i);
            }
            if (splitCounter == 1) {
                maxTemp = mlxpar.substring(j, i);
            }
            if (splitCounter == 2) {
                pixel = mlxpar.substring(j, i);
            }
            if (splitCounter == 3) {
                threshold = mlxpar.substring(j, i);
            }
              j = i + 1;
              splitCounter++;
        }
    }
    Serial.println("min Temperature: " + minTemp);
    Serial.println("max Temperature: " + maxTemp);
    Serial.println("number of pixel: " + pixel);
    Serial.println("threshold: " + threshold);
}

//count number of people on the image
void countNumberOfPeople(int count) {
    numberOfPeople = 0;
    thresholdCounter = 0;
    String XY = "";
    String coordinateXY = "";
    mlx_processed_data = "";
    int number = count - 48;
    for (int k = 1; k <= number; k++) {
        numberCounter = 0;
        int tempX = 0;
        int tempY = 0;
        for (int y = 1; y < 25; y++) {
            for (int x = 1; x < 33; x++) {
                if (image[y][x] == char(k + 48)) {
                    numberCounter ++;
                    thresholdCounter++; //to compare with threshold
                    //store x and y coordinates
                    /*
                    if (numberCounter == 1) {
                      XY =  String(x-1) + "," + String(y-1);
                    }
                    */
                    tempX = tempX + x - 1;
                    tempY = tempY + y - 1;
                }
          }
        }
        if (numberCounter >= pixel.toInt()) {
            /*
            Serial.print("tempX: ");
            Serial.println(tempX);
            Serial.print("tempY: ");
            Serial.println(tempY);
            Serial.print("numberCounter: ");
            Serial.println(numberCounter);
            */
            XY =  String(tempX/numberCounter) + "," + String(tempY/numberCounter);
            numberOfPeopleInCluster = numberCounter / pixel.toInt();
            numberOfPeople = numberOfPeople + numberOfPeopleInCluster;                      //number of people go by multiply of pixel
            coordinateXY = coordinateXY + "[" + XY + "," + numberOfPeopleInCluster + "],";
            numberOfPeopleInCluster = 0;
            //Serial.println("number of count: " + String(numberCounter));
            //Serial.println("number of pixel: " + String(pixel.toInt()));
            //Serial.println("number of peolple in pixel: " + String(numberCounter/pixel.toInt()));
        }
    }
    //Serial.print("COUNT NUMBER OF PEOPLE: ");
    //Serial.println(numberOfPeople);
    //coordinateXY = coordinateXY + "[32,24,1],[32,32,1],"; // test for max corrdinate
    mlx_processed_data = "{\"people\":" + String(numberOfPeople) + ",\"timestamp\":" + tm_epoch + ",\"coordinates\":" + "[" + coordinateXY.substring(0, coordinateXY.length() - 1) + "]," + "\"mac_address\":\"" + String(mqtt_client) + "\"}";
}

//image processing logic
void imageProcess() {
    int count = 48;
    for (int i = 1; i < 25; i++) {
        for (int j = 1; j < 33; j++) {
            if (image[i][j] == '*') {
                count++;
                image[i][j] = char(count);
                if (image[i - 1][j - 1] == '*') {
                    image[i - 1][j - 1] = char(count);
                }
                if (image[i - 1][j] == '*') {
                    image[i - 1][j] = char(count);
                }
                if (image[i - 1][j + 1] == '*') {
                    image[i - 1][j + 1] = char(count);
                }
                if (image[i][j - 1] == '*') {
                    image[i][j - 1] = char(count);
                }
                if (image[i][j + 1] == '*') {
                    image[i][j + 1] = char(count);
                }
                if (image[i + 1][j - 1] == '*') {
                    image[i + 1][j - 1] = char(count);
                }
                if (image[i + 1][j] == '*') {
                    image[i + 1][j] = char(count);
                }
                if (image[i + 1][j + 1] == '*') {
                    image[i + 1][j + 1] = char(count);
                }

                for (int topI = 1; topI < 25; topI++) {
                    for (int topJ = 1; topJ < 33; topJ++) {
                        if (image[topI][topJ] == char(count)) {
                            if (image[topI - 1][topJ - 1] == '*') {
                                image[topI - 1][topJ - 1] = char(count);
                            }
                            if (image[topI - 1][topJ] == '*') {
                                image[topI - 1][topJ] = char(count);
                            }
                            if (image[topI - 1][topJ + 1] == '*') {
                                image[topI - 1][topJ + 1] = char(count);
                            }
                            if (image[topI][topJ - 1] == '*') {
                                image[topI][topJ - 1] = char(count);
                            }
                            if (image[topI][topJ + 1] == '*') {
                                image[topI][topJ + 1] = char(count);
                            }
                            if (image[topI + 1][topJ - 1] == '*') {
                                image[topI + 1][topJ - 1] = char(count);
                            }
                            if (image[topI + 1][topJ] == '*') {
                                image[topI + 1][topJ] = char(count);
                            }
                            if (image[topI + 1][topJ + 1] == '*') {
                                image[topI + 1][topJ + 1] = char(count);
                            }
                        }
                    }
                }

                for (int topI = 1; topI < 25; topI++) {
                    for (int topJ = 33; topJ >= 1; topJ--) {
                        if (image[topI][topJ] == char(count)) {
                            if (image[topI - 1][topJ - 1] == '*') {
                                image[topI - 1][topJ - 1] = char(count);
                            }
                            if (image[topI - 1][topJ] == '*') {
                                image[topI - 1][topJ] = char(count);
                            }
                            if (image[topI - 1][topJ + 1] == '*') {
                                image[topI - 1][topJ + 1] = char(count);
                            }
                            if (image[topI][topJ - 1] == '*') {
                                image[topI][topJ - 1] = char(count);
                            }
                            if (image[topI][topJ + 1] == '*') {
                                image[topI][topJ + 1] = char(count);
                            }
                            if (image[topI + 1][topJ - 1] == '*') {
                                image[topI + 1][topJ - 1] = char(count);
                            }
                            if (image[topI + 1][topJ] == '*') {
                                image[topI + 1][topJ] = char(count);
                            }
                            if (image[topI + 1][topJ + 1] == '*') {
                                image[topI + 1][topJ + 1] = char(count);
                            }
                        }
                    }
                }

                for (int topI = 25; topI >= 1; topI--) {
                    for (int topJ = 1; topJ < 33; topJ++) {
                        if (image[topI][topJ] == char(count)) {
                            if (image[topI - 1][topJ - 1] == '*') {
                                image[topI - 1][topJ - 1] = char(count);
                            }
                            if (image[topI - 1][topJ] == '*') {
                                image[topI - 1][topJ] = char(count);
                            }
                            if (image[topI - 1][topJ + 1] == '*') {
                                image[topI - 1][topJ + 1] = char(count);
                            }
                            if (image[topI][topJ - 1] == '*') {
                                image[topI][topJ - 1] = char(count);
                            }
                            if (image[topI][topJ + 1] == '*') {
                                image[topI][topJ + 1] = char(count);
                            }
                            if (image[topI + 1][topJ - 1] == '*') {
                                image[topI + 1][topJ - 1] = char(count);
                            }
                            if (image[topI + 1][topJ] == '*') {
                                image[topI + 1][topJ] = char(count);
                            }
                            if (image[topI + 1][topJ + 1] == '*') {
                                image[topI + 1][topJ + 1] = char(count);
                            }
                        }
                    }
                }

                for (int topI = 25; topI >= 1; topI--) {
                    for (int topJ = 32; topJ >= 1; topJ--) {
                        if (image[topI][topJ] == char(count)) {
                            if (image[topI - 1][topJ - 1] == '*') {
                                image[topI - 1][topJ - 1] = char(count);
                            }
                            if (image[topI - 1][topJ] == '*') {
                                image[topI - 1][topJ] = char(count);
                            }
                            if (image[topI - 1][topJ + 1] == '*') {
                                image[topI - 1][topJ + 1] = char(count);
                            }
                            if (image[topI][topJ - 1] == '*') {
                                image[topI][topJ - 1] = char(count);
                            }
                            if (image[topI][topJ + 1] == '*') {
                                image[topI][topJ + 1] = char(count);
                            }
                            if (image[topI + 1][topJ - 1] == '*') {
                                image[topI + 1][topJ - 1] = char(count);
                            }
                            if (image[topI + 1][topJ] == '*') {
                                image[topI + 1][topJ] = char(count);
                            }
                            if (image[topI + 1][topJ + 1] == '*') {
                                image[topI + 1][topJ + 1] = char(count);
                            }
                        }
                    }
                }
            }
        }
    }
    /*
    for(int i=1;i<25;i++){
      for(int j=1;j<33;j++){
        Serial.print(image[i][j]);
      }
      Serial.println("");
    }
    */
    countNumberOfPeople(count);
}

//function to read data from MLX sensor
void MLX(String mlxMode)
{
    for (byte x = 0 ; x < 2 ; x++){ //Read both subpages
        uint16_t mlx90640Frame[834];
        int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
        if (status < 0){
            Serial.print("GetFrame Error: ");
            Serial.println(status);
        }

        float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
        float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

        float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
        float emissivity = 0.95;

        MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
    }
    // check if mlxraw is enable
    if(mlxMode == "raw"){
        Serial.println("MLX sensor get raw data");
        mlx_raw_data = "";
        int j = 0;
        for (int y = 1; y < 25; y++) {
            for (int x = 32; x >= 1; x--) {
                if ((mlx90640To[j] < minTemp.toFloat())) {image[y][x] = ' ';}
                if ((mlx90640To[j] > minTemp.toFloat()) & (mlx90640To[j] < maxTemp.toFloat())) {image[y][x] = '*';}
                if (mlx90640To[j] > maxTemp.toFloat()) {image[y][x] = ' ';}
                //Serial.print(mlx90640To[j]);
                //Serial.print(" ");
                mlx_raw_data = mlx_raw_data + String(mlx90640To[j]) + ","; //store raw data to string
                j++;
            }
            //Serial.println("");
        }
    }
    else{
        Serial.println("MLX sensor get process data");
        int j = 0;
        for (int y = 1; y < 25; y++) {
            for (int x = 32; x >= 1; x--) {
                if ((mlx90640To[j] < minTemp.toFloat())) {image[y][x] = ' ';}
                if ((mlx90640To[j] > minTemp.toFloat()) & (mlx90640To[j] < maxTemp.toFloat())) {image[y][x] = '*';}
                if (mlx90640To[j] > maxTemp.toFloat()) {image[y][x] = ' ';}
                //Serial.print(mlx90640To[j]);
                //Serial.print(" ");
                j++;
            }
            //Serial.println("");
        }
        imageProcess();
    }
}
