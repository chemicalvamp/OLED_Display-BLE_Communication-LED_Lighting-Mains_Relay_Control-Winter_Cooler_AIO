# OLED_Display-BLE_Communication-LED_Lighting-Mains_Relay_Control-Winter_Cooler_AIO
An all in one for my Arduino UNO(clone), Pro Mini(clone), and Mega(clone) projects.
Current perpetual next stage(s):
	-DeltaTime.
			DeltaTime = millis();
			DeltaTime -= LastMillis; // DeltaTime:150 = (DeltaTime:150 - 100) = 50ms
			// At end of loop:
			LastMillis = millis();
			(displaying as 0?)
		
	-PWM 12V computer case fan winter cooler climate control. (working)
	-Default On Relay controller. (working)
	-PWM breathing 12V LED strip light with (PWM problems? It appears it does nothing but 100% or 0%)
			microswitch grounded when door is open to enable maximum output and enable previously disabled strips. (working)
	