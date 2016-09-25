#include <SPI.h>
#include <WiFi.h>
#include <SoftwareSerial.h> //gps

//////////////////////////////조도//////////////////////////////
int light = A1;
int lightValue = 0;
///////////////////////////////////////////////////////////////

////////////////////////초음파/////////////////////////
int ultrasonic = 10;
//////////////////////////////////////////////////////

////////////////////GPS///////////////////////////////
SoftwareSerial gpsSerial(6,5);
//////////////////////////////////////////////////////

int status = WL_IDLE_STATUS;
char ssid[] = "ollehEgg_455"; //  your network SSID (name) 
char pass[] = "info86055"; 

IPAddress remoteIp(192,168,1,10);
int port = 65505;

String message = "";

WiFiClient client;

void setup()
{
    Serial.begin(9600);
    gpsSerial.begin(9600);
    if(WiFi.status() == WL_NO_SHIELD){
        Serial.println("WiFi shield not present! Press reset to try again.");
        while(true); //stops the program
    }
    connectWiFi();
    printWifiStatus();
    connectClient(3);
}

void loop(){  
    if(client){
      Serial.println("receivemessage");
        if(client.available()){
            Serial.println("receivemessage2");
            char c = client.read();
            while (c != '\n') {
                message +=c;
                c = client.read();
            }
           Serial.println(message);
             checkMessage(message);
             message = "";
        }
    }

////////////////////////조도///////////////////////////////
    lightValue = analogRead(light);
///////////////////////////////////////////////////////////

///////////////////////GPS////////////////////////////
  if(gpsSerial.available())
  {
    Serial.write(gpsSerial.read());
  }
//////////////////////////////////////////////////////

/////////////////////초음파///////////////////////////
long dt = 0;
  int dist = 0;

  pinMode(ultrasonic, OUTPUT);
  digitalWrite(ultrasonic, HIGH);
  digitalWrite(ultrasonic,LOW);

  pinMode(ultrasonic, INPUT);

  while(digitalRead(ultrasonic) == LOW);
  dt = micros();
  while(digitalRead(ultrasonic) == HIGH);
  dt = micros() - dt;

  dist = (333.5 + 0.60714)*((float)dt / 10000 / 2);
  Serial.println(dist);

  delay(200);
/////////////////////////////////////////////////////////

  
////////////////////////////Server전송////////////////////////
    if( status == WL_CONNECTED){
      client.print(lightValue);
      delay(5000);
      client.print(gpsSerial.read());
      delay(5000);
      client.print(dist);
      delay(5000);
    }
//////////////////////////////////////////////////////////////////    
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}

void connectWiFi(){

    if( status != WL_CONNECTED){
        while(status != WL_CONNECTED){

            Serial.print("Attempting connection to network...");

            status = WiFi.begin(ssid, pass);
            delay(3000);

            if(status == WL_CONNECTED){
                Serial.println(" SUCSESS");
            }
            else{
                Serial.println(" FAILED");
                delay(3000);
                connectWiFi();
            }
        }
    }   
}

void connectClient(int retries){

    //Attempt connection to server

    if(retries <= 0){
        Serial.println("FAILED");
        Serial.println("Connection to server failed.");
        while(true);
    }

    Serial.print("Attempting conenction to server... ");

    if(client.connect(remoteIp, port)){
        Serial.println("SUCSESS");         
        //sendMessage("Hello");
    }
    else if(retries > 0){
        Serial.println("FAILED");
        connectClient(retries - 1);
    }

}

void checkMessage(String message){

    if(message=="airOn"){
        digitalWrite(9, HIGH);
        sendMessage("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    }
    else if(message == "airOff"){
        digitalWrite(9, LOW);
        sendMessage("cccccccccccccccccccccccccccc");
    }
  //  else if (message == "ledOn"){
 //   }
 //   else if (message == "ledOff"){
  //  }
     else{
    
      Serial.println("fail");
      sendMessage("fail");
    }
}

void sendMessage(String toSend){

    if(client){
        client.println(toSend+'\n');
        client.flush();
        Serial.println("Sendt message: "+toSend);
    }
    else{
        Serial.println("Could not send message; Not connected.");
    }
    delay(1000);
}


