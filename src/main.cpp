#include <Arduino.h>


enum class LedState : uint8_t {OFF, ON};


struct AppConfig {
  static constexpr uint8_t LED_PIN = 15;
  static constexpr uint16_t BLINK_INTERVAL_MS = 1000;
  static constexpr uint32_t SERIAL_BAUD_RATE = 115200;
  static constexpr uint16_t LOOP_MEASURE_COUNT = 1000;
};


class Led {
  public:
    void init() const {
      pinMode(AppConfig::LED_PIN, OUTPUT);
      set(LedState::OFF);
    }

    void set(LedState state) const {
      digitalWrite(AppConfig::LED_PIN, state == LedState::ON ? HIGH : LOW);
    }
};

Led led;

void setup() {
  Serial.begin(AppConfig::SERIAL_BAUD_RATE);
  led.init();
}


void loop() {
  const unsigned long loopStartMicros = micros();
  static LedState state = LedState::OFF;

  static unsigned long previousMillis = 0;
  const unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= AppConfig::BLINK_INTERVAL_MS) {
    previousMillis = currentMillis;
    state = (state == LedState::OFF) ? LedState::ON : LedState::OFF;
    led.set(state);
  }

  // SuperLoop execution time measurement.
  static uint64_t totalLoopTimeMicros = 0;
  static uint16_t loopCount = 0;

  const unsigned long loopEndMicros = micros();
  const unsigned long loopDurationMicros = loopEndMicros - loopStartMicros;
  totalLoopTimeMicros += loopDurationMicros;
  ++loopCount;

  if (loopCount >= AppConfig::LOOP_MEASURE_COUNT) {
    const double averageLoopTimeMicros = static_cast<double>(totalLoopTimeMicros) / loopCount;
    Serial.print("Average loop time: ");
    Serial.print(averageLoopTimeMicros);
    Serial.println(" microseconds");

    // Reset measurement variables.
    totalLoopTimeMicros = 0;
    loopCount = 0;
  }
}

