#include <IRremote.h>
#include <U8glib.h>
#include <Wire.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
IRrecv irrecv(10);
decode_results results;
#define DK 17
#define MDOWN 16
#define MUP 15
#define RES 14
#define POWER 13
#define OK 12
#define Q 11
#define P 10
#define E 9
#define D 8
#define C 7
#define B 6
#define A 5
#define UP 4
#define DOWN 3
#define LEFT 2
#define RIGHT 1
int flappy_pos;
int wall_y;
int wall_x;
int box_x = 0;
int box_y = 0;
int box_x_length = 98;//x方向32  0-31
int box_y_length = 62;//y方向20  0-19
int snake_max_length = 100; //蛇身体最大长度
int snake_x[100];//蛇身x坐标
int snake_y[100];//蛇身y坐标
int snake_body_width = 3; //蛇身方块宽度（正方形）
int food_x;//食物位置坐标x
int food_y;//食物位置坐标y
int snake_length = 3; //当前蛇身长度
unsigned int game_speed;//游戏速度
int key_temp;//按键缓存
int web = 0;//是否开启联网
int flag;//标记
int game,mode;
int ledPin = A3;//此处接无源蜂鸣器正极
int score = 0;
int snake_dir=RIGHT;//初始方向 right
int flappy_dir=DOWN;//初始方向 down
int bingo_flag=1;
char whice_game,cscore;
void setup(void)
{
  Wire.begin();
  irrecv.enableIRIn(); // 启动红外解码
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_gdr14r);
    u8g.setPrintPos(0, 20);
    u8g.print("Arduino");
    u8g.setPrintPos(0, 35);
    u8g.print("GAME");
    u8g.setFont(u8g_font_9x18);
    u8g.setPrintPos(0, 55);
    u8g.print("By:Freddy");
  } while (u8g.nextPage());
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  pinMode(A1,INPUT);
  delay(1000);
}    
//结束
void gameover()
{

  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(score);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  score = 0;
    u8g.firstPage();
    do
    {
      u8g.setFont(u8g_font_gdr14r);
      u8g.setPrintPos(0, 40);
      u8g.print("GAME OVER!");
    } while (u8g.nextPage());  
    delay(1000);
}
//读取按键
void read_key()
{
  if (irrecv.decode(&results)){irrecv.resume();}
  if(results.value==33452175){key_temp=MDOWN;}
  else if(results.value==33486855){key_temp=MUP;}
  else if(results.value==33468495){key_temp=Q;}
  else if(results.value==33444015){key_temp=RES;}
  else if(results.value==33427695){key_temp=OK;}
  else if(results.value==33441975){key_temp=POWER;}
  else if(results.value==33464415){key_temp=UP;}
  else if(results.value==33478695){key_temp=DOWN;}
  else if(results.value==33480735){key_temp=LEFT;}
  else if(results.value==33460335){key_temp=RIGHT;}
  else if(results.value==33456255){key_temp=A;}
  else if(results.value==33439935){key_temp=B;}
  else if(results.value==33472575){key_temp=C;}
  else if(results.value==33431775){key_temp=D;}
  else if(results.value==33448095){key_temp=E;}
  else if(results.value==33435855){key_temp=P;}
  else{key_temp=DK;}
}
void bingo()
{
  int bingo_random;
  int print_pos;
  unsigned long start_time=millis();
   for(int counter=0;counter<5;counter++)
   {
     bingo_random=random(1,4.99);
     do
     {
       print_pos=60-map(analogRead(A1),0,1023,0,216);
       u8g.firstPage();
       do
       {
         u8g.setFont(u8g_font_9x18);
         u8g.setPrintPos(60,10);
         u8g.print(bingo_random);
         u8g.setPrintPos(print_pos,30);
         u8g.print("1  2  3  4  5  6  7  8  9");
       }while(u8g.nextPage());
     }while(fabs(-map(analogRead(A1),0,1023,0,216)-27+bingo_random*27)>5);
     bingo_random=random(5,9.99);
     do
     {
       print_pos=60-map(analogRead(A1),0,1023,0,216);
       u8g.firstPage();
       do
       {
         u8g.setFont(u8g_font_9x18);
         u8g.setPrintPos(60,10);
         u8g.print(bingo_random);
         u8g.setPrintPos(print_pos,30);
         u8g.print("1  2  3  4  5  6  7  8  9");
       }while(u8g.nextPage());
     }while(fabs(-map(analogRead(A1),0,1023,0,216)-27+bingo_random*27)>5);
   }
  float time_used=(millis()-start_time)/1000;
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_9x18);
    u8g.setPrintPos(0,10);
    u8g.print("Game Over");
    u8g.setPrintPos(0,25);
    u8g.print("You have used");
    u8g.setPrintPos(0,40);
    u8g.print(time_used);
    u8g.print("s");
  }while(u8g.nextPage());
  delay(3000);
  score=1000/time_used;
  gameover();
  results.value=0;
}
//游戏主界面
void snake_frame(int s)
{
  u8g.firstPage();
  do
  {
    u8g.drawFrame(box_x, box_y, box_x_length, box_y_length);
    u8g.setFont(u8g_font_5x8);
    u8g.setPrintPos(box_x_length + 1,12);
    u8g.print("Score");
    u8g.setPrintPos(box_x_length + 1, 22);
    u8g.print(score);
    u8g.setPrintPos(box_x_length + 1, 46);
    u8g.print("Speed");
    u8g.setPrintPos(box_x_length + 1, 56);
    u8g.print(30-s);
    u8g.drawFrame(food_x*snake_body_width+1, food_y*snake_body_width+1, snake_body_width, snake_body_width);//显示食物
    for (int i = 0; i < snake_length; i++)//显示snake
    {
       u8g.drawFrame(snake_x[i]*snake_body_width+1, snake_y[i]*snake_body_width+1, snake_body_width, snake_body_width);
    }
  } while (u8g.nextPage());
}
//食物随机坐标生成函数
void food()
{
    refood:
    food_x = random(0,(box_x_length-2)/3);
    food_y = random(0,(box_y_length-2)/3);
    for (int i = 0; i < snake_length; i++)
    {
        if((food_x==snake_x[i])&&(food_y==snake_y[i]))
        {
          goto refood;
        }
        flag=0;
    }
}
//移动后坐标变换函数
void change_con(int DIR)
{
  for(int i=1;i<snake_length;i++)
  {
    snake_x[snake_length-i]=snake_x[snake_length-i-1];
    snake_y[snake_length-i]=snake_y[snake_length-i-1];
  }
  if(DIR==RIGHT){snake_x[0]+=1;}
  if(DIR==LEFT){snake_x[0]-=1;}
  if(DIR==UP){snake_y[0]-=1;}
  if(DIR==DOWN){snake_y[0]+=1;}
}
//吃到食物后坐标变换
void change_pos()
{
  for(int i=1;i<snake_length;i++)
  {
    snake_x[snake_length-i]=snake_x[snake_length-i-1];
    snake_y[snake_length-i]=snake_y[snake_length-i-1];
  }
  snake_x[0]=food_x;
  snake_y[0]=food_y;
}
//判断是否吃到食物 0-没有吃到 1-吃到 2-无效
int snake_eat_food(int dir)
{
  int x_temp=snake_x[0];
  int y_temp=snake_y[0];
  switch(dir)
  {
    case UP :y_temp-=1;break;
    case DOWN :y_temp+=1;break;
    case LEFT :x_temp-=1;break;
    case RIGHT :x_temp+=1;break;
  }
  if((x_temp==food_x)&&(y_temp==food_y))
  {
    score+=1;
    if(snake_length==snake_max_length)
    {
      change_con(snake_dir);
    }
    else
    {
      snake_length+=1;
      change_pos();
      return 1;
    }
  }
  else
  {
    return 0;
  }
}
//判断是否撞墙 0-没有撞到 1-撞到 2-无效
int snake_knock_wall(int dir)
{
  int x_temp=snake_x[0];
  int y_temp=snake_y[0];
  switch(dir)
  {
    case UP :y_temp-=1;break;
    case DOWN :y_temp+=1;break;
    case LEFT :x_temp-=1;break;
    case RIGHT :x_temp+=1;break;
  }
  if(x_temp<0||x_temp>31||y_temp<0||y_temp>19)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
//判断是否吃到自己 0-没有吃到 1-吃到 2-无效
int snake_eat_body(int dir)
{
  int x_temp=snake_x[0];
  int y_temp=snake_y[0];
  switch(dir)
  {
    case UP :y_temp-=1;break;
    case DOWN :y_temp+=1;break;
    case LEFT :x_temp-=1;break;
    case RIGHT :x_temp+=1;break;
  }
  for(int i=1;i<snake_length;i++)
  {
    if((x_temp==snake_x[i])&&(y_temp==snake_y[i]))
    {
      return 1;
    }
  }
  return 0;
}
void snake()
{
  int flag = 1; //标志
  snake_length=3;
  snake_x[0]=15;snake_y[0]=15;//snake起始坐标
  snake_x[1]=snake_x[0]-1;snake_y[1]=snake_y[0];//snake起始坐标
  snake_x[2]=snake_x[1]-1;snake_y[2]=snake_y[1];//snake起始坐标
  results.value = 33460335;
  game_speed=30;
  int food_flag=1;
  food();
  while (flag){
  snake_frame(game_speed);
  delay(game_speed);
  reread_1:read_key();
  if(key_temp==P){delay(100);goto reread_1;}
  if(key_temp==UP&&snake_dir!=DOWN){snake_dir=UP;}
  if(key_temp==DOWN&&snake_dir!=UP){snake_dir=DOWN;}
  if(key_temp==LEFT&&snake_dir!=RIGHT){snake_dir=LEFT;}
  if(key_temp==RIGHT&&snake_dir!=LEFT){snake_dir=RIGHT;}
  if(snake_eat_food(snake_dir)==1){
    food();
    if(snake_length<10){game_speed=30;}
    if(snake_length<30&&snake_length>=10){game_speed=20;}
    if(snake_length<50&&snake_length>=30){game_speed=10;}
    if(snake_length>=50){game_speed=0;}}
  else if(snake_knock_wall(snake_dir)||snake_eat_body(snake_dir)||results.value == 33444015){
    gameover();
    flag=0;
    }
    else{change_con(snake_dir);}
  }
}
void choice_web()
{
  rechoice_web:u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_9x18);
    u8g.setPrintPos(5,10);
    u8g.print("A:Online");
    u8g.setPrintPos(5,25);
    u8g.print("B:Local");
  }while (u8g.nextPage());
  read_key();
  if(key_temp==A){web=1;}
  else if(key_temp==B){web=0;}
  else{goto rechoice_web;}
}
void flappy_die()
{
  int flappy_pos_temp;
  if(flappy_dir==DOWN){flappy_pos_temp=flappy_pos+1;}
  if(flappy_dir==UP){flappy_pos_temp=flappy_pos-1;}
  if(flappy_dir==DOWN&&flappy_pos==19){flag=0;}
  else if(flappy_dir==UP&&flappy_pos==0){flag=0;}
  else if(wall_x==1&&(flappy_pos_temp<wall_y||flappy_pos>wall_y+5)){flag=0;}
  else{flag=1;}
}
void flappy_pos_change()
{
  if(flappy_dir==UP){flappy_pos-=1;}
  if(flappy_dir==DOWN){flappy_pos+=1;}
  if(wall_x==0)
  {
    wall_x=31;
    wall_y=random(0,14);
  }
  else
  {
    wall_x-=1;
  }
}
void flappy_draw()
{
  u8g.firstPage();
  do
  {
    u8g.drawFrame(box_x, box_y, box_x_length, box_y_length);
    u8g.setFont(u8g_font_5x8);
    u8g.setPrintPos(box_x_length + 1,12);
    u8g.print("Score");
    u8g.setPrintPos(box_x_length + 1, 22);
    u8g.print(score);
    u8g.drawFrame(1, flappy_pos*snake_body_width+1, snake_body_width, snake_body_width);//显示bird
    for (int i = 0; i < 20; i++)//显示wall
    {
       if(i!=wall_y&&i!=wall_y+1&&i!=wall_y+2&&i!=wall_y+3&&i!=wall_y+4&&i!=wall_y+5){u8g.drawFrame(wall_x*snake_body_width+1, i*snake_body_width+1, snake_body_width, snake_body_width);}
    }
  } while (u8g.nextPage());
}
void flappy()
{
  wall_x=31;
  wall_y=random(0,14);
  flappy_pos=6;
  results.value=33478695;
  flag=1;
  flappyloop:flappy_draw();
  reread:read_key();
  if(key_temp==P){delay(100);goto reread;}
  if(key_temp==UP){flappy_dir=UP;}
  if(key_temp==DOWN){flappy_dir=DOWN;}
  flappy_die();
  if(results.value == 33444015){flag=0;}
  score+=1;
  flappy_pos_change();
  delay(100);
  if(flag==1){goto flappyloop;}
  gameover();
}
void choice_game()
{
  rechoice_game:u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_9x18);
    u8g.setPrintPos(5,10);
    u8g.print("A:Snake");
    u8g.setPrintPos(5,25);
    u8g.print("B:Flappy Bird");
    u8g.setPrintPos(5,40);
    u8g.print("C:Music");
    u8g.setPrintPos(5,55);
    u8g.print("D:Bingo");
  }while (u8g.nextPage());
  read_key();
  if(key_temp==A){game=1;}
  else if(key_temp==B){game=2;}
  else if(key_temp==C){game=3;}
  else if(key_temp==D){game=4;}
  else{goto rechoice_game;}
}
void newtone(byte tonePin, int frequency, int duration) {
int period = 1000000L / frequency;
int pulse = period / 2;
for (long i = 0; i < duration * 1000L; i += period) {
digitalWrite(tonePin, HIGH);
delayMicroseconds(pulse);
digitalWrite(tonePin, LOW);
delayMicroseconds(pulse);
}
}
void loop(void)//主循环函数
{
  reloop:choice_game();
  if(game==3)
  {
    u8g.firstPage();
    do
    {
      u8g.setFont(u8g_font_9x18);
      u8g.setPrintPos(5,10);
      u8g.print("Press Power");
      u8g.setPrintPos(5,25);
      u8g.print("Key to Quit");
    }while (u8g.nextPage());
    flag=1;
    while(flag)
    {
      results.value=0;
      read_key();
      if(key_temp==A){newtone(ledPin,262,200);}
      if(key_temp==B){newtone(ledPin,294,200);}
      if(key_temp==C){newtone(ledPin,330,200);}
      if(key_temp==D){newtone(ledPin,350,200);}
      if(key_temp==UP){newtone(ledPin,393,200);}
      if(key_temp==E){newtone(ledPin,441,200);}
      if(key_temp==LEFT){newtone(ledPin,495,200);}
      if(key_temp==OK){newtone(ledPin,525,200);}
      if(key_temp==DK){digitalWrite(ledPin,LOW);}
      if(key_temp==POWER){flag=0;}
    }
    goto reloop;
  }
  results.value = 33460335;
  choice_web();
  if(game==1){snake();}
  if(game==2){flappy();}
  if(game==4){bingo();}
}
