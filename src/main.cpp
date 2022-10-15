//Slave code
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

//Slave id ()
#define slave_id 0

//MAC to SET on slave
//uint8_t SlaveNewMACAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1}; 
uint8_t SlaveNewMACAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0}; 
//uint8_t SlaveNewMACAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF2}; 
//uint8_t SlaveNewMACAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3}; 

//Master MAC
uint8_t MasterAddress[] = {0xAA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};//ESP32}

struct struct_message {
    uint8_t id; // slave id
    uint8_t state;
};
struct_message msg;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&msg, incomingData, sizeof(msg));
  Serial.print("Bytes received: ");
  Serial.println(len);
}

void setup() {
  //Serial.begin(115200);
  msg.id = slave_id;

  //Set device as a Wi-Fi Station
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &SlaveNewMACAddress[0]);
  
  Serial.print("[NEW] ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

    if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(MasterAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}


void loop() {

  // Send message via ESP-NOW
  esp_now_send(MasterAddress, (uint8_t *) &DHTReadings, sizeof(DHTReadings));

}