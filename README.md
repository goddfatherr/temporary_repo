# firmware_implementation

Notes: 
Target Board: STM32F429ZITx.
Peripherals used: Onboard pushbutton and LED, Systick timer
I did not implement the GPIO drivers used for this purpose. I used BSP and APIs to read and write GPIO.

Logic
The program models a state machine. Here, I define 3 states 
1. Button is pressed once and LED is on (normal state)
2. Button is pressed once and LED is off (normal state)
3. Button is pressed twice, regardless of the state of LED (toggling state)

The Systick timer is used to generate interrupts every 5ms. In the interrupt handler, I polled the pushbutton every 5ms.
Debouncing was implemented in software. A button is pressed if it consecutively reads HIGH 20 times(this number is from trial-error)


Working
In the normal state, the LED is toggled upon pushbutton press. To swtich into the toggling mode, the pushbutton has to be 
presssed twice consecutively, within a 500ms interval. This is because, the value of state variable that stores the state of the button is 
automatically discarded after 500ms. This is done to minimize false positives(not to mistake two single presses for a double push)

When the LED is in toggle mode, it continues toggling until the button is pushed again, where it then goes back to the normal state.

Demo: 
https://youtube.com/shorts/Ski81yxDc8I?feature=share
https://youtube.com/shorts/WsRU6TcnSLg?feature=share




