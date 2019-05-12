/*
*
* IoT Laboratory work 1
* HTTP Server
* Variant: 11
* __________________
*
*  © 2019, Neshumov Pavel Evgenyevich
*
*/

/* ----- Libraries ----- */
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

/* ----- Pins ----- */
#define LDC_RS		2
#define LDC_E		3
#define LDC_D4		4
#define LDC_D5		5
#define LDC_D6		6
#define LDC_D7		7
#define LEDPin		8
#define SensorPin	A0

/* ----- Setup ----- */
byte mac[] = {
	0x90, 0xA2, 0xDA, 0x0D, 0xA3, 0x8
};
IPAddress ip(172, 16, 122, 62);
#define DEBUG					// Uncomment if you want to recieve debug messages by Serial

/* ----- System vars ----- */
LiquidCrystal lcd(LDC_RS, LDC_E, LDC_D4, LDC_D5, LDC_D6, LDC_D7);
EthernetServer server(80);		// HTTP Server
String header = "";
long LEDTimer = 0;

void setup() {
	pinsSetup();
	sensorsBegin();
	serverInit();
}

void loop() {
	beginServer();
	LEDAction();
	delay(100);
}


/* ----- Setup sensors ----- */
void sensorsBegin() {
	Serial.begin(9600);
	lcd.begin(16, 2);
	Ethernet.begin(mac, ip);
}

/* ----- Setup Pins ----- */
void pinsSetup() {
	pinMode(LEDPin, OUTPUT);
}

/* ----- Server init ----- */
void serverInit() {
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {	//Ethernet hardware checking
#ifdef DEBUG
		Serial.println("Ethernet shield was not found!");
#endif
		while (1);
	}
	if (Ethernet.linkStatus() == LinkOFF) {
#ifdef DEBUG
		Serial.println("Ethernet cable is not connected!");
#endif
		while (1);
	}

	server.begin();											//Starting server

#ifdef DEBUG
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
#endif
}

/* ----- Starting http server ----- */
void beginServer() {
	EthernetClient client = server.available();
	if (client) {
		Serial.println("new client");							// An http request ends with a blank line
		bool currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				header += c;
				Serial.write(c);

				if (c == '\n' && currentLineIsBlank) {
					if (header.indexOf("GET /start") >= 0) {	// If button "START" was pressed
						String value = sensorOutput();
						LEDTimer = millis();
						LEDAction();
						makeHTML(client, "PIOA31111TEMP \n SHTM31_Temp=" + value);

						lcd.setCursor(0, 1);					// Printing to LCD
						lcd.print(value);
						lcd.print("                ");			// 16 spaces

					} else										// If button wasn't pressed
						makeHTML(client, "");
					
					break;
				}
				if (c == '\n')
					currentLineIsBlank = true;
				else if (c != '\r')
					currentLineIsBlank = false;
			}
			LEDAction();
		}

		delay(1);
		client.stop();
		header = "";

#ifdef DEBUG
		Serial.println("Client disconnected!");
#endif
	}
}

/* ----- Create html web page ----- */
void makeHTML(EthernetClient client, String data) {
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	//client.println("Connection: close");
	client.println("Refresh: 5");
	client.println();
	client.println("<!DOCTYPE html>");
	client.println("<html>");
	client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
	client.println("<link rel=\"icon\" href=\"data:,\">");
	client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
	client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
	client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");
	client.println("<body><h1>Laboratory work 1</h1>");
	client.println("<h2>HTTP Web Server</h2>");
	client.println("<h1> </h1>");
	if (data.length() > 0)
		client.println("<h3>" + data + "</h3>");
	client.println("<h3>Made by Pavel Neshumov</h3>");

	client.println("<a href = \"/start\"><button class=\"button\">START</button></a></p>");
	client.println("</body></html>");
}

/* ----- LED's and sensor task for 11 variant ----- */
void LEDAction() {
	if (millis() - LEDTimer <= 500) {
		digitalWrite(LEDPin, 1);
	}
	else
		digitalWrite(LEDPin, 0);
}

String sensorOutput() {
	return analogRead(SensorPin);
}