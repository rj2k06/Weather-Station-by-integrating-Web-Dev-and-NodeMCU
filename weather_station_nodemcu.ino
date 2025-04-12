#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 2        // Pin where the DHT11 is connected
#define DHTTYPE DHT11   // DHT 11 type sensor
#define RAIN_PIN A0     // Analog pin where rain sensor is connected

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "";        // Replace with your Wi-Fi SSID
const char* password = ""; // Replace with your Wi-Fi Password

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to Wi-Fi");
  Serial.println(WiFi.localIP()); // Print the IP address
  server.begin();  // Start the server
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int rain = analogRead(RAIN_PIN);
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    // Wait until the client sends some data
    while(!client.available()){
      delay(1);
    }
    
    // Prepare the HTML response with stylish data presentation
    String html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    html += "<!DOCTYPE HTML><html><head><title>Weather Station</title>";
    
    // Adding some CSS for styling
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f2f2f2; color: #333; }";
    html += "h1 { color: #4CAF50; }";
    html += ".card { background-color: #fff; box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.1); padding: 20px; margin: 20px; border-radius: 10px; display: inline-block; }";
    html += ".data { font-size: 2em; color: #333; }";
    html += ".label { font-size: 1.2em; color: #888; }";
    html += "</style>";

    html += "</head><body>";
    html += "<h1>Weather Station Data</h1>";

    // Stylish temperature display
    html += "<div class='card'>";
    html += "<p class='data'>" + String(temperature) + "°C</p>";
    html += "<p class='label'>Temperature</p>";
    html += "</div>";

    // Stylish humidity display
    html += "<div class='card'>";
    html += "<p class='data'>" + String(humidity) + "%</p>";
    html += "<p class='label'>Humidity</p>";
    html += "</div>";

    // Stylish rain sensor display
    html += "<div class='card'>";
    html += "<p class='data'>" + String(rain) + "</p>";
    html += "<p class='label'>Rain Sensor Value</p>";
    html += "</div>";
    
    html += "</body></html>";

    // Send the HTML to the client
    client.println(html);
    
    // Close the connection
    client.stop();
  }
  
  delay(5000);  // Send data every 5 seconds
}
