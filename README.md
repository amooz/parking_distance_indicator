# parking_distance_indicator
An arduino project that indicates your position in the garage.  Starting from far away, the bottom row of pixels will flash.  As you move to within a certain distance, the pixels will switch to illuminate the 4 corners and draw a 2-pixel wide version of the car.  As you get closer the car moves up the screen. When all but the top row is illuminated you are in position.  If you get too close, only the top row will render and blink angrily at you.

Components used:
- Arduino Uno
- MAX7219 display
- Ultrasonic sensor
