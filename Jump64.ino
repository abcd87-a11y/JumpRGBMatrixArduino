#include <Adafruit_NeoPixel.h>

#define PIXPIN 7
#define NUMLEDS 64
#define JUMPBUTTON 6
#define WALKBUTTON 9
#define LED 8

const int helligkeit = 5;
Adafruit_NeoPixel matrix(NUMLEDS, PIXPIN, NEO_RGB + NEO_KHZ800);

unsigned long millis_alt[2] = { 0, 0 };
float playerX = 0.f;
float playerY = 6.f;

float velocityY = 0.f;
float gravity = 0.082f;
float jumpStrength = -0.90f;
float ground = 7.f;
bool isJumping = false;
bool farbwahl = false;


float playerSpeed = 0.01f;

float startX = 7.f;

bool gameOver = false;

void setup() {
  matrix.begin();
  matrix.setBrightness(helligkeit);
  matrix.clear();

  pinMode(JUMPBUTTON, INPUT_PULLUP);
  pinMode(WALKBUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  randomSeed(analogRead(A0));
}

int begrenzung;
int levelcnt =0;

void loop() {
  if (!gameOver) {
    matrix.clear();
    unsigned long millis_current = millis();
    unsigned long dt = millis_current - millis_alt[0];
    millis_alt[0] = millis_current;
    if (levelcnt == 0) {
      boden(0, 8);
    }
    if (levelcnt == 1) {
      boden(0, 3);
      boden(6, 8);

      if (playerX > 3 && playerX < 6 && playerY > 7) {
        gameOver = true;
      } else {
        digitalWrite(LED, LOW);
      }
    }

    if (levelcnt == 2) {
      boden(0, 8);
      float speed = -0.009f;
      for (int y = 6; y > 4; y--) {
        matrix.setPixelColor(getIndex(startX, y), 0, 0, 255);
      }
      startX += dt * speed;
      if ((int)playerX == (int)startX && playerY > 4) {
        gameOver = true;
      }
    }

    jump();

    matrix.setPixelColor(getIndex(playerX, constrain(playerY, 0, 7)), 0, 255, 0);
    if (!digitalRead(WALKBUTTON)) playerX += dt * playerSpeed;
    if (playerX > 8) {
      playerX = 0;
      levelcnt++;
    }

    /*
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
    }*/
  } else {
    digitalWrite(LED, HIGH);
  }

  matrix.show();
}

uint8_t getIndex(int x, int y) {
  return (y << 3) | x;
}

void boden(int start, int ende) {
  for (int x = start; x < ende; x++) {
    matrix.setPixelColor(getIndex(x, ground), 255, 0, 0);
  }
}

void jump() {
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

  bool ueberAbgrund = (levelcnt == 1 && playerX > 3 && playerX < 6);

  if (playerY >= ground && !ueberAbgrund) {
    playerY = 6;
    velocityY = 0.f;
    isJumping = false;
  }
}
