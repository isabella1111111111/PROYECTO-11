#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Adafruit_GFX.h>
#define PANEL_WIDTH     64
#define PANEL_HEIGHT    32
#define PANELS_NUMBER   1
HUB75_I2S_CFG mxconfig(
    PANEL_WIDTH,   // ancho
    PANEL_HEIGHT,  // alto
    PANELS_NUMBER  // cantidad de paneles
);
MatrixPanel_I2S_DMA display(mxconfig);
unsigned long lastChange = 0;
const unsigned long interval = 1000; // intervalo en milisegundos (1 segundo)
int currentNumber = 1;
#include <FS.h>
#include <SPIFFS.h>
void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }
  if (!display.begin()) {
    Serial.println("Error al inicializar el panel");
    return;
  }
  display.setBrightness8(60); // brillo entre 0 y 255
  display.clear();
  display.setTextWrap(false);  // evitar que el texto se corte
  display.setTextColor(display.color565(255, 0, 0)); // color rojo 
  display.setTextSize(5); // tamaño grande
}
void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastChange >= interval) {
    display.fillScreen(0); // limpia pantalla
    display.setCursor(10, 0); // posición del número
    // Leer el número desde bloque.txt
    File file = SPIFFS.open("/bloque.txt", "r");
    if (file) {
      String bloque = file.readString();
      int numero = bloque.toInt();
      display.print(numero);
      Serial.print("Bloque leído: ");
      Serial.println(numero);
      file.close();
    } else {
      display.print("-"); // Si no se puede leer, muestra un guion
      Serial.println("No se pudo leer bloque.txt");
    }
    lastChange = currentTime;
  }
}

