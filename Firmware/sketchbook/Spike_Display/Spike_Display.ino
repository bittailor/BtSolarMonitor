#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 sDisplay1 = Adafruit_PCD8544(A5, A4, A3);
Adafruit_PCD8544 sDisplay2 = Adafruit_PCD8544(A5, A2, A1);

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

void setup()   {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  //while (!Serial) {}
  Serial.begin(115200); 
  Serial.println(F("** DisplayCheck **"));
  digitalWrite(13, LOW);            

  sDisplay1.begin();
  sDisplay2.begin();
  Serial.println(F("** Display begin done**"));
  // you can change the contrast around to adapt the display
  // for the best viewing!
  sDisplay1.setContrast(50);
  sDisplay2.setContrast(50);

  sDisplay1.display(); // show splashscreen
  sDisplay2.display(); // show splashscreen
  delay(2000);
  sDisplay1.clearDisplay();   // clears the screen and buffer
  sDisplay2.clearDisplay();   // clears the screen and buffer

  
  // rotation example
  sDisplay1.setTextColor(BLACK);
  sDisplay1.setCursor(0,0);
  sDisplay1.println("BT Display 1");
  sDisplay1.setTextSize(2);
  sDisplay1.println("1");
  sDisplay1.display();
  
  sDisplay2.setTextColor(BLACK);
  sDisplay2.setCursor(0,0);
  sDisplay2.println("BT Display 2");
  sDisplay2.setTextSize(2);
  sDisplay2.println("2");
  sDisplay2.display();
  
  delay(4000);

}

int counter = 10000;
void loop() {
  counter++;
  
  sDisplay1.setTextSize(2);
  sDisplay1.clearDisplay(); 
  sDisplay1.setTextColor(BLACK);
  sDisplay1.setCursor(0,0);
  sDisplay1.println("1");
  sDisplay1.setTextSize(2);
  sDisplay1.println(counter);
  sDisplay1.println("ZABCDEF");
  sDisplay1.display();
  
  counter++;
  
  sDisplay2.clearDisplay();
  sDisplay2.setTextColor(BLACK);
  sDisplay2.print("I");
  sDisplay2.setTextSize(1);
  sDisplay2.print(" ");
  sDisplay2.setTextSize(2);
  sDisplay2.print((counter/1000)%100);
  sDisplay2.setTextSize(1);
  sDisplay2.print((counter%1000));
  sDisplay2.print(" ");
  sDisplay2.setTextSize(2);
  sDisplay2.println("A");
  sDisplay2.display();

  sDisplay2.print("U");
  sDisplay2.setTextSize(1);
  sDisplay2.print(" ");
  sDisplay2.setTextSize(2);
  sDisplay2.print((counter/1000)%100);
  sDisplay2.setTextSize(1);
  sDisplay2.print((counter%1000));
  sDisplay2.print(" ");
  sDisplay2.setTextSize(2);
  sDisplay2.println("V");
  sDisplay2.display();

  sDisplay2.print("L");
  sDisplay2.setTextSize(1);
  sDisplay2.print(" ");
  sDisplay2.setTextSize(2);
  sDisplay2.print((counter/1000)%100);
  sDisplay2.setTextSize(1);
  sDisplay2.print((counter%1000));
  sDisplay2.print(" ");
  sDisplay2.setTextSize(2);
  sDisplay2.println("A");
  sDisplay2.display();

  delay(200);
  
}
