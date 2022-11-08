# "Arduino Battles" Car - Team "MAY"

Arduino Battles is a competition where every team gets an Arduino exercise, 
that should be solved in a fixed time with a set of electrical components, there were three exercises that we had to solve.

## Table of contents

1. [Car details](#Car-details)
   - [Car schemes](#Car-schemes)
2. [Line Following](#Line-Following)
   - [Objective](#Objective)
   - [Solution](#Solution)
3. [Obstacles Avoidance](#Obstacles-Avoidance)
   - [Objective](#Objective-1)
   - [Solution](#Solution-1)
4. [Maze](#Maze)
   - [Objective](#Objective-2)
   - [Solution](#Solution-2)

## Car details

For all three exercises, the same scheme could be used, but the code had to be changed for the maze.

The hardware that was used in our car is:

- [Arduino Mega](https://store.arduino.cc/products/arduino-mega-2560-rev3) as the main microcontroller
- [Adafruit motor driver shield](https://learn.adafruit.com/adafruit-motor-shield) for controlling the motors
- 4 DC gear motors, for driving wheels
- 2 IR proximity sensors, for detecting colors
- 3 Ultrasonic sensors, for avoiding obstacles and going through the maze
- 2 Li-ion batteries
- A small button could be used for starting the car code using interrupts (the wires were connected and disconnected manually to emulate the button due to lack of a button)

### Car schemes

The realistic and electrical schemes of the car are shown in the pictures below:

|<img width="500" alt="Realistic scheme" src="https://user-images.githubusercontent.com/95139567/200503785-3b45ecbf-db88-420e-abe0-aa43ed756a1e.png">|<img width="500" alt="Electrical scheme" src="https://user-images.githubusercontent.com/95139567/200519102-c440f48e-4b27-4b15-ae63-af58e21d2686.png">|
|-|-|

The actual car while going through the maze is shown in the picture below:

<img width="350" alt="Maze" src="https://user-images.githubusercontent.com/95139567/200520760-ad451b44-562a-41d6-b94d-bd8a6be9e497.jpeg">

## Line Following

### Objective

The first exercise's objective is to make an Arduino car that follows a specific black line from the starting horizontal line, 
and stopping when reaching the horizontal ending line.

The path that should be followed is shown in the picture below (the car is at the start):

<img width="350" alt="Line path" src="https://user-images.githubusercontent.com/95139567/200358297-764189ba-4873-4dcd-ad64-6e019fa9dc4d.JPG">

### Solution

For solving this exercise two IR proximity sensors were placed at the front side of the car 
for detecting the black colored line, the algorithm for this exercise is really simple: 

Both sensors give their readings:
- If both sensors read non-black color, the car goes forward for a specific time,
- If the right sensor reads non-black and the left sensor reads black color, the car goes left for a specific time,
- If the left sensor reads non-black and the right sensor reads black color, the car goes right for a specific time,
- These steps are repeated until both sensors read black color, then the car stops, after that if the car was placed again on track the algorithm starts again.

The variables for this exercise were going forward time, turning time, stopping time (explained below), and car speed:
If the speed is too high the car goes off the path, if the speed (given voltage) is low the motors can't move the car easily (and the car stops eventually).

Our solution for this problem was going at a higher speed but making stops every given time, this solution made the car finish the path without mistakes, 
but the car was slow, and battery consumption was high.

Due to time limitations and technical problems (the computer had to be completely changed), this was not the best solution, but the first one to work.

## Obstacles Avoidance

### Objective

The second exercise's objective is to make an Arduino car with the same specifications as the first exercise,
in addition to that, the car has to avoid any obstacles on the path and return to the line after avoiding the obstacle.

The obstacles (books) that were placed on the same path are shown in the picture below:

<img width="350" alt="Obstacles" src="https://user-images.githubusercontent.com/95139567/200371867-357b75b4-6d30-42b1-83eb-dbe4c1971bf4.JPG">

### Solution

For avoiding the obstacles two ultrasonic sensors were used, one in the front, 
and one on the right side, the algorithm that was added for avoiding an obstacle is:
- Rotating the car 60 degrees counterclockwise,
- going forward a little (for 0.4s),
- following (going around) the obstacle by keeping a certain distance between the car and the obstacle using the right ultrasonic sensor
- when the left IR proximity sensor reads a black color the algorithm ends, and the line following continues.

This exercise had the same variables as the first one, in addition to all the variables used in the new algorithm: 
forward time, stopping time, the distance between the car and the obstacle, turning angle, etc.

In this exercise if the path around the obstacle was too wide the car would exit the given square, if it was too narrow it would hit the obstacle,
so the variables we choose were the closest we could get to a balanced solution.

The best solution for making the car go on a required path precisely was using a speed sensor (lm393 optical encoder), 
but again we didn't have enough time to add it.

## Maze

### Objective

The second exercise's objective is to make an Arduino car that goes over a maze made with books, part of the maze is shown in the picture below:

<img width="350" alt="Maze" src="https://user-images.githubusercontent.com/95139567/200379856-9ed7bde9-c03a-48ae-8804-145b16983b1b.png">

### Solution

For this exercise, three ultrasonic sensors were used, placed at the front, right, and left sides of the car. Further, books will be called walls.

The algorithm that was used is as follows:

- The car goes forward until it faces a wall,
   - If there is a way on the right side (the right sensor doesn't detect a wall), the car rotates clockwise and selects the wall on the left side of the car to be followed
   - If there is a wall on the right side, and a way on the left side, the car does alternative actions for the left way,
   - If there is a left and a right wall the car stops.
- The car follows a wall until it faces a wall, then it repeats the previous step.

For the used maze the car only followed the left side wall.

---
