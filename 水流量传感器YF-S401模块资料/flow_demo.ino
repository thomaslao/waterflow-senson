volatile float flow_frequency;
unsigned long cloopTime;

void flow () // Interrupt function
{
   flow_frequency++;
}


void setup() {
  Serial.begin(9600);
  
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), flow, RISING);
  cloopTime = millis();
}

void loop() {
  if (millis() - cloopTime >= 1000){ // Run every second
    
    // 1L = 5880 square pluse, according to the document
    // https://www.dfrobot.com/product-1531.html
    
    float L_min = flow_frequency / 5880 * 60;
    Serial.println(L_min);
  
    //Reset time and pluse counter
    cloopTime = millis();
    flow_frequency = 0;
  }

}
