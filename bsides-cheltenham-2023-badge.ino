#include <avr/io.h>-
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <avr/sleep.h>
#include <tinyNeoPixel_Static.h>

#define BUTTON_PIN PIN_PA7
#define BUMPER_POWER PIN_PA3
#define HEADLIGHT_POWER PIN_PA1
#define HEADLIGHT_GPIO PIN_PA2

#define NUMLEDS 4
#define LEFT 0
#define LEFTMID 1
#define RIGHTMID 2
#define RIGHT 3

#define OFF 0,0,0
#define RED 100,0,0
#define GREEN 0,100,0
#define BLUE 0,0,100
#define ORANGE 100,40,0
#define PINK 60,0,40
#define PURPLE 60,0,80
#define OLIVE 20,20,0
#define YELLOW 60,60,0
#define YELLOWGREEN 100,150,100
#define WHITE 80,80,100

byte pixels[NUMLEDS * 3];
tinyNeoPixel strip = tinyNeoPixel(NUMLEDS, HEADLIGHT_GPIO, NEO_GRB, pixels);

uint8_t state;

uint16_t time_pin_low(uint16_t max_ms)
{
  // blocking for up to max_ms
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    return(0);
  }
  delay(50); //debounce-
  uint16_t t = 50;
  while(digitalRead(BUTTON_PIN) == LOW)
  {
    delay(5);
    t = t + 5;
    if ( t > max_ms )
      return(max_ms);
  }
  return(t);
}

void setAllPixels(int r, int g, int b, bool show = false)
{
  for (int i = 0; i < NUMLEDS; i++) 
  {
    strip.setPixelColor(i,r,g,b);
  }
  if(show)
    strip.show();
}

void setBumper(int brightness)
{
    if (!(state & B00000001))
      return;
    analogWrite(BUMPER_POWER, brightness);
      return;
}

int police(int x)
{
  if( x % 2 == 0)
  {
    strip.setPixelColor(LEFT,RED);
    strip.setPixelColor(LEFTMID,BLUE);
    strip.setPixelColor(RIGHT,RED);
    strip.setPixelColor(RIGHTMID,BLUE);
  }
  else
  {
    strip.setPixelColor(LEFTMID,RED);
    strip.setPixelColor(LEFT,BLUE);
    strip.setPixelColor(RIGHTMID,RED);
    strip.setPixelColor(RIGHT,BLUE);
  }
  strip.show();
  return 150;
}


int chase(int x, uint8_t r,uint8_t g, uint8_t b)
{
  strip.setPixelColor((x - 1) % 5,OFF); // turn last pixel off
  strip.setPixelColor((x % 5),r,g,b); // set current pixel
  strip.show();
  return 200;
}

int pumpkin(int x)
{
  chase(x+1,2,1,0);
  chase(x,ORANGE);
  chase(x-1,2,1,0);
  return 200;
}

int rainbow(int x)
{
  //red,orange,green.blue.purple
  strip.setPixelColor((x+4) % NUMLEDS,RED);
  strip.setPixelColor((x+3) % NUMLEDS,ORANGE);
  strip.setPixelColor((x+2) % NUMLEDS,GREEN);
  strip.setPixelColor((x+1) % NUMLEDS,BLUE);
  strip.setPixelColor((x) % NUMLEDS,PURPLE);
  strip.show();
  return 300;
}

int xmas(int x)
{
  if(x % 3 == 0)
  {
    strip.setPixelColor(LEFT,RED);
    strip.setPixelColor(LEFTMID,GREEN);
    strip.setPixelColor(RIGHTMID,YELLOW);
    strip.setPixelColor(RIGHT,RED);
  }
  else if(x % 3 == 1)
  {
    strip.setPixelColor(LEFT,YELLOW);
    strip.setPixelColor(LEFTMID,RED);
    strip.setPixelColor(RIGHTMID,RED);
    strip.setPixelColor(RIGHT,GREEN);
  }
  else
  {
    strip.setPixelColor(LEFT,YELLOW);
    strip.setPixelColor(LEFTMID,GREEN);
    strip.setPixelColor(RIGHTMID,GREEN);
    strip.setPixelColor(RIGHT,YELLOW);
  }
  strip.show();
  return 300;
}

int pink(int x)
{
  if(x % 2 == 0)
  {
    strip.setPixelColor(LEFT,PINK);
    strip.setPixelColor(LEFTMID,PURPLE);
    strip.setPixelColor(RIGHTMID,PINK);
    strip.setPixelColor(RIGHT,PURPLE);
  }
  else
  {
    strip.setPixelColor(LEFTMID,PINK);
    strip.setPixelColor(LEFT,PURPLE);
    strip.setPixelColor(RIGHT,PINK);
    strip.setPixelColor(RIGHTMID,PURPLE);
  }
  strip.show();
  return 200;
}



int headlights(int x)
{
  int interval = x % 80;
  if(interval == 0)
  {
    strip.setPixelColor(LEFT,WHITE);
    strip.setPixelColor(LEFTMID,WHITE);
    strip.setPixelColor(RIGHTMID,WHITE);
    strip.setPixelColor(RIGHT,WHITE);
  }
  else if (interval == 2)
  {
    strip.setPixelColor(LEFTMID,OFF);
    strip.setPixelColor(LEFT,OFF);
    strip.setPixelColor(RIGHT,OFF);
    strip.setPixelColor(RIGHTMID,OFF);
  }
  else if (interval == 3)
  {
    strip.setPixelColor(LEFTMID,WHITE);
    strip.setPixelColor(LEFT,WHITE);
    strip.setPixelColor(RIGHT,WHITE);
    strip.setPixelColor(RIGHTMID,WHITE);
  }
  else if (interval == 40)
  {
    setBumper(255);   
  }
    else if (interval == 41)
  {
    setBumper(0);   
  }
    else if (interval == 42)
  {
    setBumper(255);   
  }
      else if (interval == 43)
  {
    setBumper(0);   
  }
    else if (interval == 44)
  {
    setBumper(255);   
  }
  else if (interval == 79)
  {
    strip.setPixelColor(LEFTMID,OFF);
    strip.setPixelColor(LEFT,OFF);
    strip.setPixelColor(RIGHT,OFF);
    strip.setPixelColor(RIGHTMID,OFF);
    setBumper(0); 
  }
  strip.show();
  return 100;
}

void success()
{
  analogWrite(PIN_PA3, 255); // SET EEPROM
  for(int j = 0; j < 4; j++)
  {
    setAllPixels(0,255,0,true);
    delay(50);
    setAllPixels(0,0,0,true);
    delay(50);
  }
  analogWrite(PIN_PA3, 0); // SET EEPROM
  EEPROM.update(0, state);
  delay(200);
}

void fail()
{
  analogWrite(PIN_PA3, 255); // SET EEPROM
  for(int j = 0; j < 4; j++)
  {
    setAllPixels(255,0,0,true);
    delay(50);
    setAllPixels(0,0,0,true);
    delay(50);
  }
  analogWrite(PIN_PA3, 0); // SET EEPROM
  delay(200);
}

int breath(uint8_t min, uint8_t max, int n, int step = 5)
{
  if (!(state & B00000001))
    return;
  int range = (max - min) / step;
  int i = n % (range * 2);
  if (i < range)
  {
    analogWrite(PIN_PA3, min + (i * step));
  }
  else
  {
    analogWrite(PIN_PA3, (max * 2) - (i * step));
  }
  return 100;
}

int flash(int x)
{
    if (!(state & B00000001))
    return;
    if(x % 2 == 0)
  {
    analogWrite(PIN_PA3,255);
  }
  else
  {
    analogWrite(PIN_PA3,0);
  }
}

int knightrider(int i, int r, int g, int b)
{
  int n = i % 7;
  setAllPixels(0,0,0,true);
  if (n > 3)
  {
    n = 3 - (n - 4);
  }
  strip.setPixelColor(n,r,g,b);
  strip.show();
  return 60;
}

int knightrider_game(int i, int target_pixel)
{
  int n = i % 7;
  setAllPixels(0,0,0,true);
  if (n > 3)
  {
    n = 3 - (n - 4);
  }
  strip.setPixelColor(n,255,0,0);
  if (n != target_pixel)
  strip.setPixelColor(target_pixel,0,5,0);
  strip.show();
  return n;
}

int level(int ms_delay, int target_pixel)
{
  int i, current_pixel;
  while(true)
  {
    for(i=0;i<8;i++)
    {
      current_pixel = knightrider_game(i, target_pixel);
      if (press_detected(ms_delay) != ms_delay)
        // a button has been pressed 
        {
          if (current_pixel == target_pixel)
            return 1;
          else
            return 0;
        }
    }
  }
}

uint16_t press_detected(uint16_t max_ms)
{
  // blocking for up to max_ms
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    return(0);
  }
  delay(10); //debounce-
  uint16_t t = 10;
  while(digitalRead(BUTTON_PIN) == HIGH)
  {
    delay(1);
    t = t + 1;
    if ( t > max_ms )
      return(max_ms);
  }
  while(digitalRead(BUTTON_PIN) == LOW)
  {
    delay(1);
    }
  return(t);
}

void playGame(int l)
{
  while(true)
  {
    if(level(l,random(0,3)) == 1)
    {
      l = l - 10;
      setAllPixels(0,255,0,true);
      delay(200);
    }
    else
    {
      // YOU LOSE
      fail();
      return;
    }
    if (l < 41)
    {
      state = state | B00000001;
      success();
      return;
    }
  }
}




void sleep()
{
  // turn off leds
  strip.setPixelColor(LEFT,OFF);
  strip.setPixelColor(LEFTMID,OFF);
  strip.setPixelColor(RIGHTMID,OFF);
  strip.setPixelColor(RIGHT,OFF);
  strip.show();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  PORTA.PIN7CTRL = PORT_PULLUPEN_bm | PORT_ISC_LEVEL_gc; // enable pullup and interrupt
  digitalWrite(HEADLIGHT_POWER, LOW);
  digitalWrite(BUMPER_POWER, LOW);
  sleep_enable();
  sleep_cpu();
  // sleep resumes here
  PORTA.PIN7CTRL = PORT_PULLUPEN_bm; // renable pullup but no interrupt
  digitalWrite(HEADLIGHT_POWER, HIGH);
}

ISR(PORTA_PORT_vect) {
  PORTA.INTFLAGS = PORT_INT7_bm; // Clear Pin 7 interrupt flag otherwise keep coming back here
}


void setup()
{
  // Power save
  // http://www.technoblogy.com/show?2RA3
  // https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/PowerSave.md
  ADC0.CTRLA &= ~ADC_ENABLE_bm; // Disable ADC
  pinMode(HEADLIGHT_POWER, OUTPUT);
  pinMode(HEADLIGHT_GPIO, OUTPUT);
  pinMode(BUMPER_POWER, OUTPUT);
  // UPDI does not need setting (PA0)

  // Pin setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(HEADLIGHT_POWER, HIGH);

  
  state = EEPROM.read(0);
  if (state == B11111111)
  {
    EEPROM.update(0, 0);
    state = EEPROM.read(0);
  }
  
}

void loop()
{
  int mode = 0;
  int interval;
  uint16_t button_low_time = 0;
  uint16_t total_interval = 0;
  strip.begin();
  int i = 0;
  while(true)
  {
    if ( mode == 0 )
    {
      interval = headlights(i);
    }
    else if ( mode == 1)
    {
      interval = police(i);
      flash(i);
    }
    else if ( mode == 2)
    {
      interval = pink(i);
      breath(0,250,i);
    }
    else if ( mode == 3)
    {
      interval = xmas(i);
    }
    else if ( mode == 4)
    {
      interval = rainbow(i);
      breath(100,250,i);
    }
        else if ( mode == 5)
    {
      interval = pumpkin(i);
    }
    else if ( mode == 6 )
    {
      interval = knightrider(i,255,0,0);
      breath(100,250,i);
    }
     else if ( mode == 7 )
    {
      interval = knightrider(i,0,255,0);
      breath(100,250,i);
    }
      else if ( mode == 8 )
    {
      interval = knightrider(i,0,0,255);
      breath(100,250,i);
    }
    else
    {
      mode = 0;
      continue;
    }
    i++;
    // This section breaks down the sleep interval to catch button presses
    total_interval = total_interval + interval;
    while(interval > 0)
    {
      delay(10);
      interval = interval - 10;
      button_low_time = time_pin_low(2000);
      if (button_low_time > 0)
      {
       /*
       * MAIN MENU
       * RAPID PRESS = CHANGE MODE
       * SHORT PRESS = ENTER GAME
       * LONG PRESS = TURN OFF
       */
        if (button_low_time > 1800)
        {
        //SLEEP
          setAllPixels(0,0,0,true);
          while(digitalRead(BUTTON_PIN) == LOW)
            delay(10);
          sleep();
        }
        else if (button_low_time > 500)
        {
          playGame(100);
        }
        else
        {
          analogWrite(BUMPER_POWER,0);
          mode = mode +1;
        }
        i = 0;
        total_interval = 0;
        // reset timer
      }
  }
  }
}