
#if LIGHTMETER
void R11()
{
//This is for LOW light condition
//Connect 1M5
//Disconnect rest
//R11 on the PCB
//pinMode (A7, INPUT);
pinMode (A4, INPUT);
pinMode (A5, OUTPUT);  // this is the one "connected"
digitalWrite (A5, LOW);
return ;
}

void R15()
{
  //This is for HIGH light condition
//Connect 750K
//Disconnect rest
//R15 on the PCB
//pinMode (A7, INPUT);
pinMode (A5, INPUT);
pinMode (A4, OUTPUT);  // this is the one "connected"
digitalWrite (A4, LOW);
return ;
}
int LightValue1()
{
R11();
//
delay (2);
int sensorValue = analogRead(lightMeter);
return sensorValue;
}
int LightValue2()
{
R15();
//
delay (2);
int sensorValue = analogRead(lightMeter);
return sensorValue;
}
#endif


