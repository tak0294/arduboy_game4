/*
Hello, World! example
June 11, 2015
Copyright (C) 2015 David Martinez
All rights reserved.
This code is the most basic barebones code for writing a program for Arduboy.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
*/

#include <Arduboy2.h>
#include "graphics.h"
float WORLD_GRAVITY = 0.1f;

/*********************************************************************************
 * MAP
 *********************************************************************************/
PROGMEM const char stage[8][16*10] = 
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
 };


/*********************************************************************************
 * Car struct.
 *********************************************************************************/
typedef struct {
  unsigned char x, y, rear_tire_angle, front_tire_angle;
  float vx,vy;
  char speed;
  short angle;
  bool is_jump;
} Car;

#define CAR_BODY_ANGLE_PADD 100


/******************************************************************************
 * GLOBAL VARS.
 ******************************************************************************/
Arduboy2 arduboy;
unsigned int frame;
Car car;
int virtual_x, virtual_y;


void initialize_car(Car *car) {
  car->x = 30;
  car->y = 30;
  car->vx = 0;
  car->vy = 0;
  car->speed = 0;
  car->angle = 0;
  car->front_tire_angle = 0;
  car->rear_tire_angle = 0;
  car->is_jump = true;
}

void draw_car(Car *car) {

  // Draw body.
  drawRect2(car->x, car->y, 70, 15, car->angle + CAR_BODY_ANGLE_PADD);
  drawRect(car->x + getCos(car->angle+295)*5, car->y + getSin(car->angle+295)*5, 6, car->angle);

  // Draw tire.
  Point rear_tire;
  Point front_tire;
  rear_tire.x = car->x + getCos(car->angle+90)*8;
  rear_tire.y = car->y + getSin(car->angle+90)*8;
  front_tire.x = car->x + getCos(car->angle)*8;
  front_tire.y = car->y + getSin(car->angle)*8;
  
  drawRect(front_tire.x, front_tire.y, 8, car->front_tire_angle);
  drawRect(rear_tire.x, rear_tire.y, 8, car->rear_tire_angle);
}

void update_car(Car *car) {
//  car->angle++;
  car->rear_tire_angle += car->speed;
  car->front_tire_angle += car->speed;

  car->x += car->vx;
  car->y += car->vy;

  // Add gravity.
  if(car->is_jump) {
    car->vy += WORLD_GRAVITY;
  }

  if(arduboy.pressed(B_BUTTON)) {
    car->speed+=2;
    if(car->speed > 100) {
      car->speed = 100;
    }
  }

  car->speed -= 1;
  if(car->speed < 0) {
    car->speed = 0;
  }

  // Hit collision with ground.
  int car_map_x = virtual_x/8 + (car->x-8)/8;
  int car_map_y = virtual_y/8 + (car->y)/8;
  arduboy.print(car_map_x);
  arduboy.print(",");
  arduboy.print(car_map_y);
  char data = pgm_read_byte(&stage[car_map_y][car_map_x]);
  arduboy.print(",");
  arduboy.print((int)data);
  
  car->angle = 0;
  if(data == 1) {
    car->is_jump = false;
    car->vy = 0;
  }else if(data == 2 || data == 3) {
    car->angle = -45;
    arduboy.print("\n");
    arduboy.print(virtual_x%8);
    car->y = car_map_y*8 - virtual_y - virtual_x%8;
  }else{

  }
//  if(arduboy.pressed(LEFT_BUTTON)) {
//    car->angle -= 2;
//    if(car->angle < 0) {
//      car->angle = 360 - car->angle;
//    }
//  }
//  if(arduboy.pressed(RIGHT_BUTTON)) {
//    car->angle += 2;
//    if(car->angle > 360) {
//      car->angle = car->angle - 360;
//    }
//  }
  
}

/******************************************************************************
 * Draw stage.
 ******************************************************************************/
void draw_stage() {
  //char data = pgm_read_byte(&tetrimino[type][spin_index][ii][jj]);
  for(int ii=0;ii<8;ii++) {
    int map_y = ii + virtual_y/8;
    for(int jj=0;jj<160;jj++) {
      int map_x = jj + virtual_x/8;
      if(map_y < 0 || map_y > 8 || map_x < 0 || map_x > 160) {
        continue;
      }
      char data = pgm_read_byte(&stage[map_y][map_x]);
      if(data == 1) {
        arduboy.drawLine(jj*8+virtual_x/8 - virtual_x%8,
                         ii*8+virtual_y/8 - virtual_y%8,
                         jj*8+virtual_x/8 - virtual_x%8 + 8,
                         ii*8+virtual_y/8 - virtual_y%8);
      }else if(data == 2) {
        arduboy.drawLine(jj*8+virtual_x/8 - virtual_x%8,
                         ii*8+virtual_y/8 - virtual_y%8,
                         jj*8+virtual_x/8 - virtual_x%8 + 8,
                         ii*8+virtual_y/8 - virtual_y%8 - 8);
      }
    }
  }
}

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.boot();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(60);

  initialize_car(&car);

  virtual_x = 0;
  virtual_y = 0;
  frame = 0;
}




// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  // first we clear our screen to black
  arduboy.clear();
  draw_stage();
  update_car(&car);
  draw_car(&car);

  if(arduboy.pressed(UP_BUTTON)) {
    virtual_y--;
  }
  if(arduboy.pressed(DOWN_BUTTON)) {
    virtual_y++;
  }

  if(arduboy.pressed(LEFT_BUTTON)) {
    virtual_x--;
  }
  if(arduboy.pressed(RIGHT_BUTTON)) {
    virtual_x++;
  }
  
  frame++;

  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}
