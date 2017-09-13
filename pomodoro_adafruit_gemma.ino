/*
   Pomodoro time management clock by Rodrigo Hernandez - November 2016
   Code modified from http://www.erikaheidi.com/blog/how-to-arduino-pomodoro-timer-part-12
   to use Adafruit's Gemma microcrontroller with Neopixels!
   Buzzer routine taken from https://learn.adafruit.com/wearable-piezo-tones-with-flora/program-it
*/
#include <Adafruit_NeoPixel.h>

// Buzzer tones
#define TONE_START  1046.50 // C
#define TONE_REST   1567.98 // G
#define TONE_END    2349.32 // d

// Colors for Neopixels
#define RED     0
#define GREEN   1
#define BLUE    2
#define YELLOW  3

// Status Pomodoro
#define IDLE_STATE  0
#define WORKING_STATE  1
#define RESTING_STATE 2

int timerState = IDLE_STATE;
int BRIGHTNESS = 255;

#define PIN 1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

const int BUTTON = 2;
const int BUZZER = 0;

// Pomodoro working and resting times
const unsigned long POMO_DURATION_MS = 1500000; // 25 minutes
const unsigned long POMO_RESTING_MS = 300000; // 5 minutes

unsigned long time;
unsigned long pomo_timer;

int pomo_state = IDLE_STATE;

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUTTON, LOW);
  digitalWrite(BUTTON, HIGH); // Turn on pullup resistors
  strip.begin();
  strip.show();
  strip.setPixelColor(0, 0, 0, 255);
  strip.show();
  buzz(TONE_START);
  pomo_timer = millis();
}

void buzz(int note) {
  long time_value = 500000;
  long time_elapsed = 0;
  while (time_elapsed < time_value) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(note / 2);

    digitalWrite(BUZZER, LOW);
    delayMicroseconds(note / 2);
    time_elapsed += note;
  }
}

void changeStatus(int new_status) {
  if (new_status == WORKING_STATE) {
    pomo_state = WORKING_STATE;
    pomodoroPulse(RED);
  }
  if (new_status == RESTING_STATE) {
    pomo_state = RESTING_STATE;
    pomodoroPulse(GREEN);
  }
  if (new_status == IDLE_STATE) {
    pomo_state = IDLE_STATE;
    pomodoroPulse(BLUE);
  }
}

void pomodoroPulse(int color) {
  // Fading Neopixels
  strip.show();
  for (int led_count = 0; led_count <= 3; led_count++) {
    for (int counter = 0; counter <= BRIGHTNESS; counter++) {
      int button_state = digitalRead(BUTTON);
      if (button_state == LOW) {
        break;
      }
      if (color == RED) {
        strip.setPixelColor(led_count, strip.Color(counter, 0, 0));
      }
      if (color == GREEN) {
        strip.setPixelColor(led_count, strip.Color(0, counter, 0));
      }
      if (color == BLUE) {
        strip.setPixelColor(led_count, strip.Color(0, 0, counter));
      }
      if (color == YELLOW) {
        strip.setPixelColor(led_count, strip.Color(counter, counter, 0));
      }
      strip.show();
      delay(5);
    }
    for (int counter = BRIGHTNESS; counter >= 0; counter--) {
      int button_state = digitalRead(BUTTON);
      if (button_state == LOW) {
        break;
      }
      if (color == RED) {
        strip.setPixelColor(led_count, strip.Color(counter, 0, 0));
      }
      if (color == GREEN) {
        strip.setPixelColor(led_count, strip.Color(0, counter, 0));
      }
      if (color == BLUE) {
        strip.setPixelColor(led_count, strip.Color(0, 0, counter));
      }
      if (color == YELLOW) {
        strip.setPixelColor(led_count, strip.Color(counter, counter, 0));
      }
      strip.show();
      delay(5);
    }
  }
}

void pomodoroPulseAllFlashing(int color) {
  // Fading Neopixels
  strip.show();
  for (int counter = 0; counter <= BRIGHTNESS; counter++) {
    int button_state = digitalRead(BUTTON);
    if (button_state == LOW) {
      break;
    }
    if (color == RED) {
      strip.setPixelColor(0, strip.Color(counter, 0, 0));
      strip.setPixelColor(1, strip.Color(counter, 0, 0));
      strip.setPixelColor(2, strip.Color(counter, 0, 0));
      strip.setPixelColor(3, strip.Color(counter, 0, 0));
    }
    if (color == GREEN) {
      strip.setPixelColor(0, strip.Color(0, counter, 0));
      strip.setPixelColor(1, strip.Color(0, counter, 0));
      strip.setPixelColor(2, strip.Color(0, counter, 0));
      strip.setPixelColor(3, strip.Color(0, counter, 0));
    }
    if (color == BLUE) {
      strip.setPixelColor(0, strip.Color(0, 0, counter));
      strip.setPixelColor(1, strip.Color(0, 0, counter));
      strip.setPixelColor(2, strip.Color(0, 0, counter));
      strip.setPixelColor(3, strip.Color(0, 0, counter));
    }
    if (color == YELLOW) {
      strip.setPixelColor(0, strip.Color(counter, counter, 0));
      strip.setPixelColor(1, strip.Color(counter, counter, 0));
      strip.setPixelColor(2, strip.Color(counter, counter, 0));
      strip.setPixelColor(3, strip.Color(counter, counter, 0));
    }
    strip.show();
    delay(5);
  }
  for (int counter = BRIGHTNESS; counter >= 0; counter--) {
    int button_state = digitalRead(BUTTON);
    if (button_state == LOW) {
      break;
    }
    if (color == RED) {
      strip.setPixelColor(0, strip.Color(counter, 0, 0));
      strip.setPixelColor(1, strip.Color(counter, 0, 0));
      strip.setPixelColor(2, strip.Color(counter, 0, 0));
      strip.setPixelColor(3, strip.Color(counter, 0, 0));
    }
    if (color == GREEN) {
      strip.setPixelColor(0, strip.Color(0, counter, 0));
      strip.setPixelColor(1, strip.Color(0, counter, 0));
      strip.setPixelColor(2, strip.Color(0, counter, 0));
      strip.setPixelColor(3, strip.Color(0, counter, 0));
    }
    if (color == BLUE) {
      strip.setPixelColor(0, strip.Color(0, 0, counter));
      strip.setPixelColor(1, strip.Color(0, 0, counter));
      strip.setPixelColor(2, strip.Color(0, 0, counter));
      strip.setPixelColor(3, strip.Color(0, 0, counter));
    }
    if (color == YELLOW) {
      strip.setPixelColor(0, strip.Color(counter, counter, 0));
      strip.setPixelColor(1, strip.Color(counter, counter, 0));
      strip.setPixelColor(2, strip.Color(counter, counter, 0));
      strip.setPixelColor(3, strip.Color(counter, counter, 0));
    }
    strip.show();
    delay(5);
  }
}

void loop() {
  // If button is pressed, change status
  int button_state = digitalRead(BUTTON);
  if (button_state == LOW) {
    // Button pressed
    if (pomo_state == IDLE_STATE) {
      // Changing from IDLE to WORKING
      buzz(TONE_START);
      pomo_state = WORKING_STATE;
      pomo_timer = millis(); //reset timer
    }
    else if (pomo_state == WORKING_STATE) {
      // Changing from WORKING to RESTING
      buzz(TONE_REST);
      pomo_state = RESTING_STATE;
      pomo_timer = millis();
    }
    else if (pomo_state == RESTING_STATE) {
      // Changing from RESTING to IDLE
      buzz(TONE_END);
      pomo_state = IDLE_STATE;
      pomo_timer = millis();
    }
    delay(100);
  }
  else {
    time = millis() - pomo_timer;
    if (pomo_state == WORKING_STATE) {
      changeStatus(WORKING_STATE);
      if (time > POMO_DURATION_MS) {
        // Changing from WORKING to RESTING
        buzz(TONE_REST);
        changeStatus(RESTING_STATE);
        pomo_timer = millis();
      }
    }
    else if (pomo_state == RESTING_STATE) {
      changeStatus(RESTING_STATE);
      if (time > POMO_RESTING_MS) {
        // Changing FROM RESTING to IDLE
        buzz(TONE_END);
        changeStatus(IDLE_STATE);
        pomo_timer = millis();
      }
    }
    else if (pomo_state == IDLE_STATE) {
      changeStatus(IDLE_STATE);
    }
  }
  delay(50);
}

