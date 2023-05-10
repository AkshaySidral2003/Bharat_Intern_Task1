/*
Bharat Intern Task 1: Smart Parking System

Componets Used in this system:
1) Arduino Uno
2) ESp8266
3) IR Sensor

*/

#include <SoftwareSerial.h>

#define RX 9
#define TX 10
#define DEBUG true

SoftwareSerial esp8266(RX, TX);


const char ssid = "Redmi 6 Pro";
const char password = "12349876";

//Pins of IR sensors
int D0= 2,D1=3,D2=4,D3=5;

String page = "";

// No. of IR sensors = No. of data variables
//In my case i have used 4 IR sensor so i have declared 4 data variables
String data1, data2, data3, data4;
void setup()
{
  //  Pin numbers of IR sensors and making it as input
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  delay(1000);
  Serial.begin(115200);

  sendData("AT+RST\r\n", 2000, DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n", 1000, DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // turn on server on port 80
}

void loop()
{
  //According to no. of sensors, use there many if condition
  //In my case, i have used 4 sensor,  each sensors has 2 posiblities either it
  //shows available or Not available, so there are 8 posiblities for 4 sensors
  if (digitalRead(D0) == 0)
  {
    data1 = "Not Available";
  }
  if (digitalRead(D0) == 1)
  {
    data1 = "Available";
  }
  if (digitalRead(D1) == 0)
  {
    data2 = "Not Available";
  }
  if (digitalRead(D1) == 1)
  {
    data2 = "Available";
  }
  if (digitalRead(D2) == 0)
  {
    data3 = "Not Available";
  }
  if (digitalRead(D2) == 1)
  {
    data3 = "Available";
  }
  if (digitalRead(D3) == 0)
  {
    data4 = "Not Available";
  }
  if (digitalRead(D3) == 1)
  {
    data4 = "Available";
  }

  if (esp8266.available()) // check if the esp is sending a message

  {

    if (esp8266.find("+IPD,"))

    {

      delay(1000);

      int connectionId = esp8266.read() - 48; /* We are subtracting 48 from the output because the read() function returns the ASCII decimal value and the first decimal number which is 0 starts at 48*/
      page = "<html><head><center><meta http-equiv=\"refresh\" content=\"2\"> <html> <h1>PARKING SLOTS</h1><h2>Slot 1:</p><h2>" + String(data1) + ":</p><hr><h2>Slot 2:</p><h2>" + String(data2) + ":</p><hr><h2>Slot 3</p><h2>" + String(data3) + "</p><hr><h2>Slot 4</p><h2>" + String(data4) + "</p><hr></body></center></html>";
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += page.length();
      cipSend += "\r\n";
      sendData(cipSend, 1000, DEBUG);
      sendData(page, 1000, DEBUG);
      cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += page.length();
      cipSend += "\r\n";
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
      sendData(closeCommand, 3000, DEBUG);
    }
  }
}
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command); // send the read character to the esp8266
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
