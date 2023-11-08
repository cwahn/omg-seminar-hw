#include <Arduino.h>

// Define the tactile switch pin and motor control pins
const int switch_pin = 33;            // Example GPIO pin for the switch
const int motor_pwm_pin_forward = 25; // Example PWM pin for motor forward
const int motor_pwm_pin_reverse = 26; // Example PWM pin for motor reverse

// Global instance of the motor speed controller
MotorSpeedController motor_speed_controller;

// Function to control motor speed and direction based on a speed value from -1 to 1
void set_motor_speed(float speed)
{
    int speed_forward = 0;
    int speed_reverse = 0;
    if (speed > 0)
    {
        speed_forward = static_cast<int>(speed * 255);
    }
    else
    {
        speed_reverse = static_cast<int>(-speed * 255);
    }

    // Write to the PWM pins
    analogWrite(motor_pwm_pin_forward, speed_forward);
    analogWrite(motor_pwm_pin_reverse, speed_reverse);
}

// ISR for the tactile switch
void IRAM_ATTR handle_switch_interrupt()
{
    EdgeType edge = digitalRead(switch_pin) == HIGH ? EdgeType::Rising : EdgeType::Falling;
    float speed = motor_speed_controller.react(edge, millis());
    set_motor_speed(speed);
}

void setup()
{
    Serial.begin(115200);

    // Set up the switch pin with an internal pull-up resistor
    pinMode(switch_pin, INPUT_PULLUP);
    // Attach the interrupt to the handle_switch_interrupt function
    attachInterrupt(digitalPinToInterrupt(switch_pin), handle_switch_interrupt, CHANGE);

    // Set up the PWM output for motor control
    pinMode(motor_pwm_pin_forward, OUTPUT);
    pinMode(motor_pwm_pin_reverse, OUTPUT);
}

void loop()
{
    // Nothing to do here - everything is handled by interrupts and the motor control function
}
