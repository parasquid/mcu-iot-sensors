require "pigpio"
include Pigpio::Constant

require_relative "config/pin_mappings"

pi = Pigpio.new
unless pi.connect
  exit -1
end

leds = [
  pi.gpio(D9),
  pi.gpio(D10),
  pi.gpio(D11),
  pi.gpio(D12),
  pi.gpio(D13),
]
leds.each do |led|
  led.mode = PI_OUTPUT
  led.pud = PI_PUD_OFF

  3.times do |i|
    led.write 1
    sleep 0.05
    led.write 0
    sleep 0.05
  end
end

button1 = pi.gpio(D2)
button1.mode = PI_INPUT
button1.pud = PI_PUD_UP

button2 = pi.gpio(D3)
button2.mode = PI_INPUT
button2.pud = PI_PUD_UP

# reset the buzzer
buzzer = pi.gpio(D5)
buzzer.mode = PI_OUTPUT
buzzer.pud = PI_PUD_OFF
buzzer.pwm.dutycycle = 0

led_r = pi.gpio(D9)
led_r.mode = PI_OUTPUT
led_r.pud = PI_PUD_OFF
pwm = led_r.pwm

i = 0
while true
  puts "#{button1.read} #{button2.read}"
  i = i + 1 if button1.read == 0 && i < 256
  i = i - 1 if button2.read == 0 && i > 0
  pwm.dutycycle = i
  sleep 0.01
end

pi.stop
