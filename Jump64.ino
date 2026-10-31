#include <Adafruit_NeoPixel.h>

#define PIXPIN 7
#define NUMLEDS 64
#define JUMPBUTTON 6
#define LED 8

const int helligkeit = 5;
Adafruit_NeoPixel matrix(NUMLEDS, PIXPIN, NEO_RGB + NEO_KHZ800);

unsigned long millis_alt[2] = { 0, 0 };
float playerX = 0;
float playerY = 6;

float velocityY = 0.f;
float gravity = 0.08f;
float jumpStrength = -1.2f;
float ground = 7.f;
bool isJumping = false;
bool farbwahl = false;

float speed = -0.009f;
float startX = 7.f;

bool gameOver = false;

void setup() {
  matrix.begin();
  matrix.setBrightness(helligkeit);
  matrix.clear();

  pinMode(JUMPBUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  randomSeed(analogRead(A0));
}

int g;
int r;
int b;

int begrenzung;

void loop() {
  if (!gameOver) {
    matrix.clear();
    unsigned long millis_current = millis();
    unsigned long dt = millis_current - millis_alt[0];
    millis_alt[0] = millis_current;
    
    boden();
    jump();

    matrix.setPixelColor(getIndex(playerX, constrain(playerY, 0, 6)), 0, 255, 0);
    if (!farbwahl) {
      begrenzung = random(1, 6);
      do {
        g = random(0, 256);
        r = random(0, 256);
        b = random(0, 256);
      } while (g + r + b < 255);
      farbwahl = true;
    }
    for (int y = 6; y > begrenzung; y--) {
      matrix.setPixelColor(getIndex(startX, y), g, r, b);
    }

    startX += dt * speed;
    if (startX <= 0 && playerY > begrenzung) {
      gameOver = true;
      digitalWrite(LED, HIGH);
    }
    if (startX < 0) {
      startX = 6;
      farbwahl = false;
      //speed -= 0.0005;
    }
  }
  matrix.show();
}

uint8_t getIndex(int x, int y) {
  return (y << 3) | x;
}

void boden(){
  for(int x = 0; x < 8; x++){
      matrix.setPixelColor(getIndex(x, 7), 255, 0, 0);
    }
}

void jump(){
  if (!digitalRead(JUMPBUTTON) && !isJumping) {
      velocityY = jumpStrength;
      isJumping = true;
    }

    if (millis() - millis_alt[1] > 30) {
      if (!digitalRead(JUMPBUTTON)) velocityY += gravity;
      else velocityY += 0.2;
      playerY += velocityY;
      millis_alt[1] = millis();
    }

    if (playerY >= ground) {
      playerY = ground;
      velocityY = 0.f;
      isJumping = false;
    }
}

