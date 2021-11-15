//www.elegoo.com
//2016.12.08

const int MasterRelayPin = 5; //represents the master switch relay signal being sent out to the Crydom.
const int U11_Error = 8; //this is our "weBad" signal from the DCP. Force a full power restart instantly 
const int U12_Okay = 9;//this we represent the "we good" signal sent from the DCP. This is our check in saying I got through the scan. It doesn't actually check if instruments are working. That is the loggers job.

int NotOkayCnt; //the number of scans that have passed with a "WeGood" check-in signal
int ErrorFlag; //we got the "Webad" signal from the CR6
int ErrorThreshold; //what is the number of scans without a "weGood" check-in that will trigger a reset


void setup() 
{
  pinMode(MasterRelayPin, OUTPUT);//this pin represents power getting sent to the master relay switch
  pinMode(U11_Error, INPUT); 
  pinMode(U12_Okay, INPUT);
  NotOkayCnt = 0; //start at 0 on startup or if we lose power
  ErrorFlag = 0;  //on startup make sure the flag is set to 0 so we don't flip the HMS
  ErrorThreshold = 28800; //two hours for testing purposes is 28800
}

void loop() 
{
  if (digitalRead(U11_Error) == HIGH)//if we get the "we bad" signal trip the relay
  {
    ErrorFlag = 1;
  }
  if (digitalRead(U12_Okay) == LOW)//we did not recieve a "weGood" check-in from the CR6
  {
   NotOkayCnt++; //add one to the counter
  }
  if (digitalRead(U12_Okay) == HIGH)//the CR6 sent a "weGood" check-in so reset the counter
  {
   NotOkayCnt=0; //counter is reset to zero since we got a positive signal from the CR6
  }
if (NotOkayCnt >= ErrorThreshold or ErrorFlag == 1) //if we get a we bad signal, trip the relay. If we do not get a "weGood" signal check-in we are not communicating with the CR6 anymore, trip the relay after a threshold is met
  {
    digitalWrite(MasterRelayPin, HIGH); //power sent to crydon and relay is tripped
    NotOkayCnt = 0; //since we triggered the master relay reset our counter to zero
    ErrorFlag = 0; //again resetting the counter so it doesn't happen next time too
    delay(60000); //keep relay tripped for a set time. This interval will need to be adjusted to make sure the whole board shuts down. 60000 is one minute
    digitalWrite(MasterRelayPin, LOW); //turn relay off
  }
  delay(250);//this is the only way to control the scan. We need to make a lot of small scans. Ideally multiple small scan per 1min scan on the CR6. Just to make sure the arduino is looking for input while the CR6 is actually sending inforamtion to a pin.
}
