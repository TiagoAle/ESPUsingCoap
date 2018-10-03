/*
ESP-COAP Server
*/

#include <ESP8266WiFi.h>
#include <coap_server.h>


// CoAP server endpoint url callback
void callback_light(coapPacket &packet, IPAddress ip, int port, int obs);

coapServer coap;

//WiFi connection info
const char* ssid = "AndroidPP";
const char* password = "15042018";

// LED STATE
bool LEDSTATE;
bool LEDSTATE2;
bool LEDSTATE3;

// CoAP server endpoint URL
void callback_light(coapPacket *packet, IPAddress ip, int port,int obs) {
  Serial.println("light");

  // send response
  char p[packet->payloadlen + 1];
  memcpy(p, packet->payload, packet->payloadlen);
  p[packet->payloadlen] = NULL;
  Serial.println(p);

  String message(p);

  if (message.equals("0"))
  {
    digitalWrite(16,LOW);
    Serial.println("if loop");
  }
  else if (message.equals("1"))
  {
    digitalWrite(16,HIGH);
    Serial.println("else loop");
  }
  char *light = (digitalRead(16) > 0)? ((char *) "1") :((char *) "0");

   //coap.sendResponse(packet, ip, port, light);
   if(obs==1)
    coap.sendResponse(light);
   else
    coap.sendResponse(ip,port,light);

}



void callback_lightled(coapPacket *packet, IPAddress ip, int port,int obs) {
  Serial.println("Lightled");

  // send response
  char p[packet->payloadlen + 1];
  memcpy(p, packet->payload, packet->payloadlen);
  p[packet->payloadlen] = NULL;

  String message(p);

  if (message.equals("0"))
    LEDSTATE = false;
  else if (message.equals("1"))
    LEDSTATE = true;

  if (message.equals("2"))
    LEDSTATE2 = false;
  else if (message.equals("3"))
    LEDSTATE2 = true;

  if (message.equals("4"))
    LEDSTATE3 = false;
  else if (message.equals("5"))
    LEDSTATE3 = true;

  if (LEDSTATE) {
    digitalWrite(4, HIGH) ;
    if(obs==1)
     coap.sendResponse("1");
     else
    coap.sendResponse(ip, port, "1");

    //coap.sendResponse("1");
  } else {
    digitalWrite(4, LOW) ;
    if (obs==1)
    coap.sendResponse("0");
    else
    coap.sendResponse(ip, port, "0");
    //coap.sendResponse("0");
  }

  if (LEDSTATE2) {
    digitalWrite(5, HIGH) ;
    if(obs==1)
     coap.sendResponse("3");
     else
    coap.sendResponse(ip, port, "3");

    //coap.sendResponse("1");
  } else {
    digitalWrite(5, LOW) ;
    if (obs==1)
    coap.sendResponse("2");
    else
    coap.sendResponse(ip, port, "2");
    //coap.sendResponse("0");
  }

  if (LEDSTATE3) {
    digitalWrite(16, HIGH) ;
    if(obs==1)
     coap.sendResponse("5");
     else
    coap.sendResponse(ip, port, "5");

    //coap.sendResponse("1");
  } else {
    digitalWrite(16, LOW) ;
    if (obs==1)
    coap.sendResponse("4");
    else
    coap.sendResponse(ip, port, "4");
    //coap.sendResponse("0");
  }
}

void setup() {
  yield();
  //serial begin
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println(" ");

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    yield();
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());

  // LED State
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  LEDSTATE = true;
  LEDSTATE2 = true;
  LEDSTATE3 = true;

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  //LEDSTATE = true;

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);


  // add server url endpoints.
  // can add multiple endpoint urls.

  coap.server(callback_light, "light");
  coap.server(callback_lightled, "lightled");
 // coap.server(callback_text,"text");

  // start coap server/client
  coap.start();
  coap.start(5683);
}

void loop() {
  coap.loop();
  //delay(1000);


}
