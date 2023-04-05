#include <Cthulhu.h>

/*
  This example shows how to use Sapien LLC's Cthulhu Sheild to electrotactiley stimulate the tongue.
  In this example, one electrode is set active, and five stimulation parameters are set. 
  The Cthulhu library is initialized, and serial output is enabled. 
  The CheckWaveform function is used to verify that we have entered a valid waveform for all electrodes,
  if the paramters we entered are OK, then the stimulus waveform is updated (UpdateStimuli), and the pulsetrain is sent to the 
  active electrodes (Stimulate) for one stimulation cycle (36ms). 
  If the waveform is invalid, the error code is printed to the serial monitor and the stimulus is not updated or produced. 
  This loops indefinitely.
*/

/*
  Cthulhu Tongue Stimulation BareMinimum Example - Example for stimulating the tongue using electrotactile stimulation.
  Created by Joel Moritz Jr Feb 2019
  Released into the public domain.
*/
String data = "";

int array[18];
int p1=0;
int p2=0;
int p3=0;
int p4=0;
int p5=0;
int p44 = 123;



Cthulhu mycthulhu; //creating and instance of Cthulhu

//array to hold which electrodes should be on or off. In this example, one electrode is turned on.
int trodes[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

//pulse period for each electrode, in microseconds. Can be manipulated with Pp and IN to change the intensity of the sensation.
int  PP[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,20,0}; 

//length of positive pulse for each electrode, in microseconds. Can be manipulated with PP and IN to change the intensity of the sensation.
int  Pp[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0};

//inner burst number (how many pulses in each inner burst). Can be manipulated with PP and Pp to change the intensity of the sensation.
int  IN[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0};

//inner burst period.  In microseconds. Can change quality, or 'feel' of sensation.
int  IP[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,900,900,0}; 

//Outer burst number. Can change quality, or 'feel' of sensation.
int  ON[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0}; 


//Requirements:
//Pp must be less than PP.                    CheckWaveform Error 1.
//(PP*IN) must be less than IP.               CheckWaveform Error 2.
//IP*IN must be less than 2000 microseconds.  CheckWaveform Error 3.


void setElectrodeIntensity(int e /*num*/, int i/*intencity*/) {

  PP[e]=p1;
  Pp[e]=p2;
  IN[e]=p3;
  if (p44 != 123){
    IP[e]=p4*10 + p44;
      
  }else{
    IP[e]=p4;
  }
  ON[e]=p5;
  

}


void setup() {
  mycthulhu.Begin(); //Initialize Cthulhu library
  Serial.begin(9600); //Set serial output datarate
}



void loop() 
{
  if(Serial.available()) //monitor the serial line, update stimulus parameters, and continuously stimulate.
  { // If the bluetooth sent any characters
    readPacket();
    for (int e = 0; e < 18; e++)
      {
      array[e] = 0;
      }
    p1 = (int)data[0]; //PP
    p2 = (int)data[1]; //Pp
    p3 = (int)data[2]; //IN
    p4 = (int)data[3]; //IP
    p44 = (int)data[4]; //IP2 
    p5 = (int)data[5]; //ON
    if (p44==1){
      p44=0;
    }
    for (int e = 6; e < data.length(); e=e+1)
      {
      array[(int)data[e]-1] = 1;  
      setElectrodeIntensity((int)data[e]-1, 0);
      
    }
        
    Serial.println(data.length());

    //Serial.println(data.length());
    mycthulhu.UpdateStimuli(array, PP, Pp, IN, IP, ON);
  }   
  else 
  {
    mycthulhu.Stimulate();
  }
      
}


void readPacket() 
{
  data = "";
  while(1) 
  {
    int current = Serial.read();
    if(current != 0) {
      data += (char)current;
    }
    else 
    {
      data += '\0';
      break;
    }
  }
}
