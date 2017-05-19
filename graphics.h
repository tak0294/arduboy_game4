#ifndef GRAPHICS_H
#define GRAPHICS_H

extern Arduboy2 arduboy;

PROGMEM const short SHORT_SIN[] = {
  0,
524,
1047,
1570,
2093,
2615,
3136,
3656,
4175,
4693,
5209,
5724,
6237,
6749,
7258,
7765,
8269,
8771,
9271,
9767,
10261,
10751,
11238,
11722,
12202,
12679,
13151,
13620,
14084,
14544,
15000,
15451,
15898,
16339,
16776,
17207,
17634,
18054,
18470,
18880,
19284,
19682,
20074,
20460,
20840,
21213,
21580,
21941,
22294,
22641,
22981,
23314,
23640,
23959,
24271,
24575,
24871,
25160,
25441,
25715,
25981,
26239,
26488,
26730,
26964,
27189,
27406,
27615,
27816,
28007,
28191,
28366,
28532,
28689,
28838,
28978,
29109,
29231,
29344,
29449,
29544,
29631,
29708,
29776,
29836,
29886,
29927,
29959,
29982,
29995,
30000,
};


float getSin(int angle) {
  angle+=45;
// 90-180 → sin( 180 - θ )
// 180-270 → -sin( θ - 180 )
// 270-360 → -sin( 360 - θ )
  short target_value;
  angle = abs(angle%360);
  if(angle < 90) {
    target_value = pgm_read_dword(&SHORT_SIN[angle]);
  }else if(angle < 180) {
    target_value = pgm_read_dword(&SHORT_SIN[180 - angle]);
  }else if(angle < 270) {
    target_value = pgm_read_dword(&SHORT_SIN[angle - 180]) * -1;
  }else{
    target_value = pgm_read_dword(&SHORT_SIN[360 - angle]) * -1;
  }
//  short target_value = pgm_read_dword(&SHORT_SIN[angle%360]);
  return (float)target_value / 30000.0f;
}

float getCos(int angle) {
//  short target_value = pgm_read_dword(&SHORT_COS[angle%360]);
//  return (float)target_value / 30000.0f;
  return getSin(angle + 90);
}


void drawTriangle(int x, int y, int size, int angle) {
  float half_size = size/2;

  Point p1,p2,p3;

  p1.x = x + half_size * (getCos(angle));
  p1.y = y + half_size * (getSin(angle));

  p2.x = x + half_size * (getCos(120+angle));
  p2.y = y + half_size * (getSin(120+angle));

  p3.x = x + half_size * (getCos(240+angle));
  p3.y = y + half_size * (getSin(240+angle));


  // P1-P2
  arduboy.drawLine(p1.x, p1.y, p2.x, p2.y);
  // P2-P3
  arduboy.drawLine(p2.x, p2.y, p3.x, p3.y);
  // P3-P1
  arduboy.drawLine(p3.x, p3.y, p1.x, p1.y);

}

void drawRect2(int x,int y,int min_angle, int size,int angle) { 
  
  float half_size = size/2;

  Point tl,tr,bl,br;

  tl.x = x + half_size * getCos(angle);
  tl.y = y + half_size * getSin(angle);

  tr.x = x + half_size * getCos(min_angle+angle);
  tr.y = y + half_size * getSin(min_angle+angle);

  bl.x = x + half_size * getCos(180+angle);
  bl.y = y + half_size * getSin(180+angle);
//
  br.x = x + half_size * getCos(min_angle+angle+180);
  br.y = y + half_size * getSin(min_angle+angle+180);

  // Top
  arduboy.drawLine(tl.x, tl.y, tr.x, tr.y);
  // Bottom
  arduboy.drawLine(bl.x, bl.y, br.x, br.y);
  // Left.
  arduboy.drawLine(tl.x, tl.y, br.x, br.y);
  // Right.
  arduboy.drawLine(tr.x, tr.y, bl.x, bl.y);

}

void drawRect(int x,int y,int size,int angle) {

  float half_size = size/2;

  Point tl,tr,bl,br;

  tl.x = x + half_size * getCos(angle);
  tl.y = y + half_size * getSin(angle);

  tr.x = x + half_size * getCos(90+angle);
  tr.y = y + half_size * getSin(90+angle);

  bl.x = x + half_size * getCos(180+angle);
  bl.y = y + half_size * getSin(180+angle);
//
  br.x = x + half_size * getCos(270+angle);
  br.y = y + half_size * getSin(270+angle);

  // Top
  arduboy.drawLine(tl.x, tl.y, tr.x, tr.y);
  // Bottom
  arduboy.drawLine(bl.x, bl.y, br.x, br.y);
  // Left.
  arduboy.drawLine(tl.x, tl.y, br.x, br.y);
  // Right.
  arduboy.drawLine(tr.x, tr.y, bl.x, bl.y);


}
#endif
