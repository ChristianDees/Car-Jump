# Car Jump

## Description
Car Jump is a versatile program designed for the MSP430 microcontroller, transforming it into a multifunctional device that features both an engaging game and a practical clock.

## How to Play
Once passed the loading screen, press SW2 to display the game controls. When playing Car Jump, your objective is to leap over oncoming cars, skillfully avoiding collisions to keep the game going. This adds a dynamic challenge as you try to achieve higher scores with each attempt.

## Compilation Instructions
The Makefile in the current directory builds the Car Jump program, timer library, and the lcd library using **make all**. To load the program onto the MSP430, use **make load** in the **carJumpGame** directory. Clean both directories with **make clean** in the original directory.

```bash
make all
cd carJumpGame/
make load
cd ..
make clean
```

## Compatibility
This program is built to be compatible with the MSP430Gxxx series chips, partnered with the Educational BoosterPack found [here](https://www.tindie.com/products/robg/educational-boosterpack-service/). As the program is tailored to the specific model of LCD board and MSP430, it requires board-specific features to ensure the proper functioning of the application.
