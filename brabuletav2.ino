/* Sketch baseado no código de Bill Earl:
 *  https://learn.adafruit.com/multi-tasking-the-arduino-part-1/a-clean-sweep
 *  E sketch Strandtest, da biblioteca Adafruit_NeoPixel
 *  Modificado e adaptado por Gedeane Kenshima
 *  27/06/2020
 */
#include <Servo.h> 
#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, 2, NEO_GRB + NEO_KHZ800);
unsigned long interval=50;  // intervalo de 50 ms

// Função para efeitos no NeoPixel
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Classe para servos
class Sweeper
{
  Servo servo;              // the servo
  int pos;              // posição do servo
  int increment;        // incremento para mover a cada intervalo
  int  updateInterval;      // intervalo entre updates
  unsigned long lastUpdate; // última atualização
 
public: 
  Sweeper(int interval){
    updateInterval = interval;
    increment = 1;
  }
  
  void Attach(int pin){
    servo.attach(pin);
  }
  
  void Detach(){
    servo.detach();
  }
  
  void Update()
  {
    // Variável de contagem j iniciado em 0
    static uint8_t j = 0;
    // Função millis para contagem de tempo
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
      pos += increment;
      servo.write(pos);
      strip.setBrightness(80);
      // Posições iniciais e finais dos servos
      if ((pos >= 70) || (pos <= 0)) 
      {
        // Muda direção de movimento
        increment = -increment;
      }
      // Contador para acender NeoPixels
      for (uint16_t Pixel = 0; Pixel < strip.numPixels(); Pixel++){       
          strip.setPixelColor(Pixel, Wheel(((Pixel * 256 / strip.numPixels()) + j) & 255));
        }
        // Acende NeoPixels
        strip.show();
        // Incrementa variável j
        j++;
     }
  }
};
 
// Objetos sweeper1 e 2, referente à classe Sweeper 
Sweeper sweeper1(25);
Sweeper sweeper2(25);
 
void setup() { 
  // Objetos anexados aos pinos dos servos
  sweeper1.Attach(8);
  sweeper2.Attach(9);
  // Início dos efeitos NeoPixel
  strip.begin();
  strip.show();
} 
 
void loop() { 
  // Função para servos e NeoPixels
  sweeper1.Update();
  sweeper2.Update();
}
