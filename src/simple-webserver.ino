#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Husarnet.h>
#include <WiFi.h>

#define HTTP_PORT 8080

#if __has_include("credentials.h")

// For local development (rename credenials-template.h and type your WiFi and
// Husarnet credentials there)
#include "credentials.h"

#else

// For GitHub Actions OTA deploment

// WiFi credentials
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;

// Husarnet credentials
const char *hostName = HUSARNET_HOSTNAME;
const char *husarnetJoinCode = HUSARNET_JOINCODE;  // find at app.husarnet.com
const char *dashboardURL = "default";

#endif

AsyncWebServer server(HTTP_PORT);

// index.html available in "index_html" const String
extern const char index_html_start[] asm("_binary_src_index_html_start");
const String index_html = String((const char*)index_html_start);

void setup(void) {
  // ===============================================
  // Wi-Fi, OTA and Husarnet VPN configuration
  // ===============================================

  // remap default Serial (used by Husarnet logs) 
  Serial.begin(115200, SERIAL_8N1, 16, 17);  // from P3 & P1 to P16 & P17
  Serial1.begin(115200, SERIAL_8N1, 3, 1);  // remap Serial1 from P9 & P10 to P3 & P1

  Serial1.println("\r\n**************************************");
  Serial1.println("GitHub Actions OTA example");
  Serial1.println("**************************************\r\n");

  // Init Wi-Fi
  Serial1.printf("📻 1. Connecting to: %s Wi-Fi network ", ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    static int cnt = 0;
    delay(500);
    Serial1.print(".");
    cnt++;
    if (cnt > 10) {
      ESP.restart();
    }
  }

  Serial1.println(" done\r\n");

  // Init Husarnet P2P VPN service
  Serial1.printf("⌛ 2. Waiting for Husarnet to be ready ");

  Husarnet.selfHostedSetup(dashboardURL);
  Husarnet.join(husarnetJoinCode, hostName);
  Husarnet.start();

  // Before Husarnet is ready peer list contains: 
  // master (0000:0000:0000:0000:0000:0000:0000:0001)
  const uint8_t addr_comp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  bool husarnetReady = 0;
  while (husarnetReady == 0) {
    Serial1.print(".");
    for (auto const &host : Husarnet.listPeers()) {
      if (host.first == addr_comp) {
        ;
      } else {
        husarnetReady = 1;
      }
    }
    delay(1000);
  }

  Serial1.println(" done\r\n");

  // define HTTP API for remote reset
  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Reseting ESP32 after 1s ...");
    Serial1.println("Software reset on POST request");
    delay(1000);
    ESP.restart();
  });

  // Init OTA webserver (available under /update path)
  AsyncElegantOTA.begin(&server);
  server.begin();

  // ===============================================
  // PLACE YOUR APPLICATION CODE BELOW
  // ===============================================

  // Example webserver hosting table with known Husarnet Hosts
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });

  Serial1.println("🚀 HTTP server started\r\n");
  Serial1.printf("Visit:\r\nhttp://%s:%d/\r\n\r\n", hostName, HTTP_PORT);

  Serial1.printf("Known hosts:\r\n");
  for (auto const &host : Husarnet.listPeers()) {
    Serial1.printf("%s (%s)\r\n", host.second.c_str(), host.first.toString().c_str());
  }
}

void loop(void) { ; }