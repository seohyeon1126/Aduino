#include <Servo.h>

// Arduino pin assignment
#define PIN_LED   9   // LED active-low
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servo motor

// configurable parameters for sonar
#define SND_VEL 346.0       // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25         // sampling interval (unit: msec)
#define PULSE_DURATION 10   // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 180.0     // minimum distance to be measured (unit: mm)
#define _DIST_MAX 360.0     // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficient to convert duration to distance

// Target Distance
#define _RANGE_LOW 180.0  // 18cm in mm
#define _RANGE_HIGH 360.0 // 36cm in mm

// duty duration for myservo.writeMicroseconds()
#define _DUTY_MIN -1000   // servo 0° position
#define _DUTY_NEU 1000    // servo 90° position
#define _DUTY_MAX 3000    // servo 180° position

#define _EMA_ALPHA 0.3    // optimal alpha for smoothing out noise

// global variables
float dist_ema, dist_prev = _RANGE_HIGH; // unit: mm
unsigned long last_sampling_time;        // unit: ms

Servo myservo;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);    // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);     // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 

  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);

  dist_prev = _RANGE_LOW;       // initialize distance

  Serial.begin(57600);
}

void loop() {
  float dist_raw;

  if (millis() < (last_sampling_time + INTERVAL))
    return;

  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  if (dist_raw >= _RANGE_LOW && dist_raw <= _RANGE_HIGH) {
    dist_prev = dist_raw;
    digitalWrite(PIN_LED, LOW); 
  } else {
    digitalWrite(PIN_LED, HIGH); 
  }

  dist_ema = _EMA_ALPHA * dist_prev + (1 - _EMA_ALPHA) * dist_ema;

  int servo_position;
  if (dist_ema <= _RANGE_LOW) {
    servo_position = _DUTY_MIN; // 0°
  } else if (dist_ema >= _RANGE_HIGH) {
    servo_position = _DUTY_MAX; // 180°
  } else {
    servo_position = map(dist_ema, _RANGE_LOW, _RANGE_HIGH, _DUTY_MIN, _DUTY_MAX);
  }
  myservo.writeMicroseconds(servo_position);

  Serial.print("Measured Distance: "); Serial.print(dist_raw);
  Serial.print(", Filtered Distance: "); Serial.print(dist_ema);
  Serial.print(", Servo Position: "); Serial.println(servo_position);
  
  last_sampling_time += INTERVAL;
}

float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
