# OLED_Display-BLE_Communication-LED_Lighting-Mains_Relay_Control-Winter_Cooler_AIO
An all in one for my Arduino UNO(clone), Pro Mini(clone), and Mega(clone) projects.
Current perpetual next stage(s):
	!-DeltaTime problem: its displaying 0 even with a 350ms delay in loop plus 5*10 delay in TemperatureFunction?
			DeltaTime = millis();
			DeltaTime -= LastMillis; // DeltaTime:150 = (DeltaTime:150 - 100) = 50ms
			// In TemperatureFunction()
			for (SampleIndex = 0; SampleIndex < SamplesCount; SampleIndex++) 
			{
				Samples[SampleIndex] = analogRead(ThermisterPin);
				delay(10);
			}
			// At end of loop:
			LastMillis = millis();
	-PWM 12V computer case fan winter cooler climate control. (working)
	-Default On Relay controller. (working)
	-PWM breathing 12V LED strip light (working)
	-Microswitch grounded when door is open to enable maximum output and enable previously disabled strips. (working)
	
Future goals and current specs:
	-Ogrmar SSR-25 DA 25A 3-32V DC / 24-380V AC Solid State Relay and Heat Sink
		https://www.amazon.com/gp/product/B074FT4VXB/
	-KEDSUM HC-06 Serial Slave Module, Wireless RF Transceiver Module
		https://www.amazon.com/gp/product/B0093XAV4U/
	-HiLetgo Uno R3 Proto Shield Prototype Expansion Board with SYB-170 Mini Breadboard Based for Arduino UNO ProtoShield
		https://www.amazon.com/gp/product/B00HHYBWPO
	-Logging Recorder Shield Data Logger Module For Arduino UNO SD Card // battery, SDcard, and timeing componants
		https://usa.banggood.com/Logging-Recorder-Shield-Data-Logger-Module-For-Arduino-UNO-SD-Card-p-929174.html
	-Wemos® Mega +WiFi R3 ATmega2560+ESP8266 32Mb Memory HUB Arduino to communicate between server and bluetooth recievers.
		https://usa.banggood.com/Wemos-Mega-WiFi-R3-ATmega2560ESP8266-32Mb-Memory-USB-TTL-CH340G-Compatible-For-Arduino-Mega-p-1205437.html
	-Cylewet RFP30N06LE 30A 60V 0.047Ohm Logic Level N-Channel Power Mosfet TO-220 ESD Rated for Arduino
		https://www.amazon.com/Cylewet-RFP30N06LE-0-047Ohm-N-Channel-Arduino/dp/B06W9F1QGF
	-MintCell 120mm Case Fan 4-Pack, Long Life Hydraulic Bearing Computer Case Fan, Ultra High Air Flow Computer Cooling Standard Case Fan Perfect for Mining Rigs
		https://www.amazon.com/gp/product/B078NGHXT2/
	-12V LED Strip Light, Flexible, SMD 2835, 300 LEDS, 16.4ft Tape Light for Home, blah blah, Non-waterproof, Daylight White
		https://www.amazon.com/gp/product/B00WQFP3AO/
	-HiLetgo 2.4 Inch TFT LCD Display Shield Touch Panel ILI9341 240X320 for Arduino. // to display a scrollable button panel 
		ebay mega-deals-online
	-server interface with webcam 1920*1080 display as well as a carbon copy of the HUB's touch screen scaled up to match the streams width.
	-AD623 Programmable Gain Digital Potentiometer Module Instrumentation Amplifier
		ebay alice1101983
	-GY-521 MPU-6050 MPU6050 3 Axis Accelerometer Gyroscope Module 6 DOF 6-axis Accelerometer Gyroscope Sensor Module 16 Bit AD Converter Data Output IIC I2C
		https://www.amazon.com/gp/product/B008BOPN40/
	-Digital 38khz Infrared Ir Sensor Transmitter Kit for Arduino
		https://www.amazon.com/gp/product/B011TWNMKW/
	-Nema 17 Stepper Motor Bipolar 2A 59Ncm(84oz.in) 48mm Body 4-lead W/ 1m Cable and Connector compatible with 3D Printer/CNC
		https://www.amazon.com/gp/product/B00PNEQKC0/
	-L298N H-bridge Motor Controller, DROK L298N Motor Driver Board DC Dual H Bridge Stepper Motor Regulator and Drives Module for Arduino
		https://www.amazon.com/gp/product/B00CAG6GX2/