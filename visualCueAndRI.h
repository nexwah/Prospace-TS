void redVisualCue(){
    //red led LDR0 and LDR1
    Wire.beginTransmission(0x62);
    Wire.write(0x08);
    Wire.write(0x25);
    Wire.endTransmission();
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
}

void greenVisualCue(){
    //green led LDR0 and LDR2
    Wire.beginTransmission(0x62);
    Wire.write(0x08);
    Wire.write(0x19);
    Wire.endTransmission();
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
}

void offVisualCue(){
    //off led LDR0, LDR1 and LDR2
    Wire.beginTransmission(0x62);
    Wire.write(0x08);
    Wire.write(0x15);
    Wire.endTransmission();
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
}

void redRI(){
    //red light
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
    digitalWrite(red, HIGH);
}

void greenRI(){	//NO part
    //green light
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
}

void yellowRI(){	//NO part
    //yellow light
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    digitalWrite(blue, LOW);
}

void offRI(){	//NO part
    //off light
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
}
