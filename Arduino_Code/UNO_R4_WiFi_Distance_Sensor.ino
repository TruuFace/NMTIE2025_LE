#include <WiFiS3.h>

// === WiFi Access Point credentials ===
const char* ap_ssid = "Arduino-Distance-Sensor";
const char* ap_password = "demo1234";

// === Hardware pins ===
const int trigPin = A0;
const int echoPin = A1;
const int led1 = 10;  // red
const int led2 = 11;  // orange
const int led3 = 12;  // purple
const int led4 = 13;  // blue
const int buzzer = 5;

// === Distance thresholds (cm) ===
const int RANGE1 = 15;
const int RANGE2 = 10;
const int RANGE3 = 5;

// === State variables ===
long currentDistance = -1;
bool led1State = false;
bool led2State = false;
bool led3State = false;
bool led4State = false;
bool buzzerState = false;

// === WiFi server ===
WiFiServer server(80);

// === Timing ===
unsigned long lastPrint = 0;
const unsigned long printDelay = 5000;

void setup() {
  Serial.begin(115200);
  
  // Pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);
  
  Serial.println("Ultrasonic demo with WiFi Access Point!");
  
  // Create WiFi Access Point
  Serial.print("Creating Access Point: ");
  Serial.println(ap_ssid);
  
  int status = WiFi.beginAP(ap_ssid, ap_password);
  
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed!");
    while (true) {
      delay(1000);
    }
  }
  
  delay(1000);
  
  Serial.println("Access Point created successfully!");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("Password: ");
  Serial.println(ap_password);
  Serial.print("Dashboard URL: http://");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

long readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;
  long cm = duration * 0.0343 / 2;
  return cm;
}

void updateLEDsAndBuzzer(long dist) {
  led1State = false;
  led2State = false;
  led3State = false;
  led4State = false;
  buzzerState = false;
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  noTone(buzzer);
  
  if (dist < 0) {
    // Out of range
  }
  else if (dist > RANGE1) {
    digitalWrite(led1, HIGH);
    led1State = true;
  }
  else if (dist > RANGE2) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    led1State = true;
    led2State = true;
  }
  else if (dist > RANGE3) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    tone(buzzer, 900, 100);
    led1State = true;
    led2State = true;
    led3State = true;
    buzzerState = true;
  }
  else {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    tone(buzzer, 1500);
    led1State = true;
    led2State = true;
    led3State = true;
    led4State = true;
    buzzerState = true;
  }
}

void handleWebRequest() {
  WiFiClient client = server.available();
  
  if (client) {
    String request = "";
    bool currentLineIsBlank = true;
    unsigned long timeout = millis();
    
    while (client.connected() && (millis() - timeout < 100)) {
      if (client.available()) {
        char c = client.read();
        request += c;
        
        if (c == '\n' && currentLineIsBlank) {
          
          // Check if this is a data request
          if (request.indexOf("GET /data") >= 0) {
            // Send JSON data
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.print("{\"dist\":");
            client.print(currentDistance);
            client.print(",\"led1\":");
            client.print(led1State ? "1" : "0");
            client.print(",\"led2\":");
            client.print(led2State ? "1" : "0");
            client.print(",\"led3\":");
            client.print(led3State ? "1" : "0");
            client.print(",\"led4\":");
            client.print(led4State ? "1" : "0");
            client.print(",\"buzz\":");
            client.print(buzzerState ? "1" : "0");
            client.println("}");
          } else {
            // Send HTML page
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            
            client.print("<!DOCTYPE html><html><head><meta charset='UTF-8'>");
            client.print("<meta name='viewport' content='width=device-width,initial-scale=1'>");
            client.print("<title>Distance Sensor</title>");
            client.print("<style>");
            client.print("body{font-family:Arial;background:linear-gradient(135deg,#1e3c72,#2a5298);color:#fff;text-align:center;padding:20px;margin:0}");
            client.print("h1{font-size:28px;margin:20px;text-shadow:2px 2px 4px #000}");
            client.print(".org{color:#ff6b35;font-size:16px;margin:10px;letter-spacing:2px}");
            client.print(".dist{font-size:60px;font-weight:bold;margin:30px;padding:20px;background:rgba(255,255,255,0.1);border-radius:15px}");
            client.print(".leds{display:flex;justify-content:center;flex-wrap:wrap;margin:20px}");
            client.print(".led{width:70px;height:70px;border-radius:50%;margin:10px;border:3px solid #fff;transition:all 0.3s}");
            client.print(".on{box-shadow:0 0 20px currentColor}");
            client.print(".off{opacity:0.3}");
            client.print(".buzz{font-size:40px;margin:20px}");
            client.print("</style></head><body>");
            
            client.print("<h1>Distance Sensor</h1>");
            client.print("<div class='org'>GAIA Makerspace</div>");
            client.print("<div class='org'>UTEP Learning Environments</div>");
            
            client.print("<div class='dist' id='dist'>-- cm</div>");
            
            client.print("<div class='leds'>");
            client.print("<div class='led off' id='led1' style='background:red'></div>");
            client.print("<div class='led off' id='led2' style='background:orange'></div>");
            client.print("<div class='led off' id='led3' style='background:purple'></div>");
            client.print("<div class='led off' id='led4' style='background:blue'></div>");
            client.print("</div>");
            
            client.print("<div class='buzz' id='buzz'>🔇 OFF</div>");
            
            client.print("<script>");
            client.print("function update(){");
            client.print("fetch('/data').then(r=>r.json()).then(d=>{");
            client.print("document.getElementById('dist').innerText=(d.dist>=0?d.dist+' cm':'-- cm');");
            client.print("document.getElementById('led1').className='led '+(d.led1?'on':'off');");
            client.print("document.getElementById('led2').className='led '+(d.led2?'on':'off');");
            client.print("document.getElementById('led3').className='led '+(d.led3?'on':'off');");
            client.print("document.getElementById('led4').className='led '+(d.led4?'on':'off');");
            client.print("document.getElementById('buzz').innerText=d.buzz?'🔊 ON':'🔇 OFF';");
            client.print("});");
            client.print("}");
            client.print("setInterval(update,1000);");
            client.print("update();");
            client.print("</script>");
            
            client.print("</body></html>");
          }
          break;
        }
        
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    
    delay(1);
    client.stop();
  }
}

void loop() {
  currentDistance = readDistance();
  
  if (millis() - lastPrint >= printDelay) {
    Serial.print("Distance: ");
    Serial.print(currentDistance);
    Serial.println(" cm");
    lastPrint = millis();
  }
  
  updateLEDsAndBuzzer(currentDistance);
  handleWebRequest();
  
  delay(150);
}
