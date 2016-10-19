#include <CurieBLE.h>
/**
 * Example demonstrating blinking LEDs with Web Bluetooth
 * There is nothing specific in this sketch for the Web Bluetooth API.
 * The Web Bluetooth specific portion is contained on the web site
 * for this "Physical Web" device. After constructing the circuit and
 * uploading the sketch to your arduino/genuio101, log onto this website 
 * with your Chrome browser:
 * 
 *    https://lachendekatze.github.io/wb-blink101/
 *    
 *      and click away!   
 */

// pins for each LED specified here
#define BLECONNECT 3

#define YELLOWLED 4 
#define BLUELED 5
#define GREENLED 6
#define REDLED 7

class LED{

  int pin;
  boolean ledOn;
  int rateIndex;
  long lastSwitch;

  long rates[4] = {0,250,150,50};
  
  public:

  LED(int pinNumber)
  {
      pin = pinNumber;
  }

  /**
  *
  * init() takes care of both hardware and software initilizaton of the LED
  */
  void init()
  {
    ledOn = false;
    rateIndex = 0;
    lastSwitch = millis();
    
    pinMode(pin,OUTPUT);
    digitalWrite(pin,ledOn);  
  }
  
  /**
  *
  * setRate() will be called each time the BLE charactertistic is written to.
  * as there is a latency due to the BLE stack, I fully expect that we won't
  * see a immediate response to the setRate() function.
  */
  void setRate() 
  {
    if (rateIndex == 3)
    { 
      rateIndex = 0;
      ledOn = false;
      digitalWrite(pin,ledOn);
    }
    else
    {
      rateIndex = rateIndex + 1; 
      ledOn = true; 
    }
  }

  void update()
  {

    if (rateIndex != 0) { 
      if ((millis()-lastSwitch) > (rates[rateIndex]))
      {
        lastSwitch = millis();
        ledOn = !ledOn;
        digitalWrite(pin,ledOn);              
      }
    }
  }

 /**
  *  utility function to turn of LEDs and reset states
  */
  
}; 

/* establish BLE service & characteristics */
BLEPeripheral blePeripheral;
BLEService ledService("917649a3-d98e-11e5-9eec-0002a5d5c51b");
BLEUnsignedCharCharacteristic ledCharacteristic("917649a4-d98e-11e5-9eec-0002a5d5c51b", BLEWrite);
BLEDescriptor exp3Descriptor("2902","exp");

LED redLED(REDLED);
LED greenLED(GREENLED);
LED blueLED(BLUELED);
LED yellowLED(YELLOWLED);

void setup() 
{
  // initiate serial communications for debugging
  Serial.begin(9600);
  Serial.println("web bluetooth-arduino101 blink example");
  
 /** 
   *  BLE initilizations
   */

  blePeripheral.setLocalName("blink");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(ledCharacteristic);
  blePeripheral.addAttribute(exp3Descriptor);
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  ledCharacteristic.setEventHandler(BLEWritten, ledCharacteristicWritten);

  //setup the BLE Conect LED
  pinMode(BLECONNECT, OUTPUT);

  // All BLE characteristics should be initialized to a starting value prior
  // using them.
  redLED.init();
  greenLED.init();
  blueLED.init();
  yellowLED.init();
  
  ledCharacteristic.setValue(0);
  blePeripheral.begin();  
}

void loop() 
{
  // blePeripheral.poll();
  redLED.update();
  greenLED.update();
  blueLED.update();
  yellowLED.update();
}

void blePeripheralConnectHandler(BLECentral& central) 
{
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  //may notice slight lag/latency between connect and LED on
  digitalWrite(BLECONNECT, HIGH);
}

void blePeripheralDisconnectHandler(BLECentral& central) 
{
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  digitalWrite(BLECONNECT, LOW);
}

void ledCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic)
{
  /**
   * toggle LEDs based on selection
   */
  int ledData = ledCharacteristic.value();
  switch(ledData)
  {
    case 0:
      // 0 value, do nothing
      Serial.print(ledData);
      break;
    case 1:
      // Yellow LED
      Serial.print("Yellow LED ");Serial.println(ledData);
      yellowLED.setRate();
      break;
    case 2:
      // Blue LED
      Serial.print("Blue LED ");Serial.println(ledData);
      blueLED.setRate();
      break;   
    case 3:
      // Green LED
      Serial.print("Green LED ");Serial.println(ledData);
      greenLED.setRate();
      break;  
    case 4:
      // Red LED
      Serial.print("Red LED ");Serial.println(ledData);
      redLED.setRate();
      break;  
  }

}  
