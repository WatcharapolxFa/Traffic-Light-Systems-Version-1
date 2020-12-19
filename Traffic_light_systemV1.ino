/*MYBOTIC TRAFFIC LIGHT PEDESTRIAN CONTROL*/
#include <Servo.h>
#include <Wire.h>
#include <hd44780.h> // main hd44780 header
#include <hd44780_I2Cexp.h> // i2c expander i/o class header
Servo myservo; //ประกาศตัวแปรแทน Servo
Servo myservo2; //ประกาศตัวแปรแทน Servo2
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

int buzzer_module = 53;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

//ไฟอันซ้าย
#define LIGHT_1_RED 2
#define LIGHT_1_YELLOW 3
#define LIGHT_1_GREEN 4

//ไฟอันขวา
#define LIGHT_2_RED 5
#define LIGHT_2_YELLOW 6
#define LIGHT_2_GREEN 7

//ไฟเดิน
#define LIGHT_Walk_GREEN 12
#define LIGHT_Walk_RED 9

// ตัวSensor
#define echoPin 44 // Echo Pin
#define trigPin 45 // Trigger Pin
#define LEDPin 13 // Onboard LED
int maximumRange = 10; // ระยะสูงสุดของsensor
int minimumRange = 5; // ระยะต่ำสุดของsensor
long duration, distance; // Duration used to calculate distance คำนวนระยะ

bool sensor_ON = false; 
bool shownum = false; //เช็คค้างเลขตอน Interrupt
////////////////////////////////////////////////////////////////////////////////
// 7Segment อันขวา
  int segmentA = 22;
  int segmentB = 23;
  int segmentC = 24; 
  int segmentD = 25;
  int segmentE = 26;
  int segmentF = 27;
  int segmentG = 28;
  //int segmentDP = 13;
// 7Segment อันซ้าย
  int segmentA2 = 34;
  int segmentB2 = 35;
  int segmentC2 = 36; 
  int segmentD2 = 37;
  int segmentE2 = 38;
  int segmentF2 = 39;
  int segmentG2 = 40;
  //int segmentDP = 13;
// 7Segment 1 digit
  int segmentA3 = 46;
  int segmentB3 = 47;
  int segmentC3 = 48;
  int segmentD3 = 49;
  int segmentE3 = 50;
  int segmentF3 = 51;
  int segmentG3 = 52;
  //int segmentDP = 53;
  
  //digit1-2 อันขวามือ หลักหน่วยเป็น1 หลักสิบเป็น2  
  int digit1 = 29;
  int digit2 = 30;
  //digit3-4 อันซ้ายมือ หลักหน่วยเป็น3 หลักสิบเป็น4  
  int digit3 = 41;
  int digit4 = 42; 

  int count = 0; //ตัวนับวินาทีอันขวา
  int count2 = 0; //ตัวนับวินาทีอันซ้าย
  int d1=0,d2=3,d3=0,d4=3; //d2 คือหลักสิบอันขวา d4คือหลักสิบอันซ้าย
  int yellow1_count;
  int yellow2_count;
  int check = 1; // เช็คไฟเขียวไฟเเดง
  int set_check = 0; // เก็บค่าเช็คไฟเขียวไฟเเดง
  int walk_num; // เลข9-0
  void displaySegment(int); // 7 segment อันขวา
  void displaySegment2(int); // 7 segment อันซ้าย
  void displaySegment3(int); // 7 segment 1 digit
  void displayDigit(int);
  
  bool light_RG = true; // เช็คไฟเเดงเขียว
  bool light_GR = false; // เช็คไฟเขียวเเดง
  bool set_RG = false; //เก็บค่าไฟเเดงเขียว
  bool set_GR = false; //เก็บค่าไฟเขียวเเดง
  bool count1_ON = true; //เช็คตัวนับ เอาไว้ค้างเลข
  bool red1_ON = false;
  bool yellow1_ON = false;
  bool green1_ON = false;
  bool still_green1 = false;

  bool count2_ON = true; //เช็คตัวนับ เอาไว้ค้างเลข
  bool red2_ON = false;
  bool yellow2_ON = false;
  bool green2_ON = false;
  bool still_green2 = false;

  bool walk_Yellow = false;
  bool walk_Red = false;
  bool walk_checklight1 = false; // เช็คไฟตอนเดิน
  bool walk_checklight2 = false; // เช็คไฟตอนเดิน

  bool start_count1 = true; //เช็คตัวนับถอยหลังอัน1
  bool start_count2 = true;  //เช็คตัวนับถอยหลังอัน2
  bool Ser_work = false;
  bool light1_red = false; // ไฟซ้ายเเดง ไฟขวาเขียว ไม่มีการเดิน
  bool light2_red = false; // ไฟซ้ายเขียว ไฟขวาเเดง ไม่มีการเดิน
  bool light1_2_red = false; // ไฟซ้ายเเดง ไฟขวาเเดง มีการเดิน

  bool walk_1time = false; // เช็๕การinterruptว่าต้องเกิดเเค่1ครั้ง
  void interrupt_walk()
  {   
      
       digitalWrite(trigPin, LOW);
       delayMicroseconds(2);
       digitalWrite(trigPin, HIGH);
       delayMicroseconds(2);
       digitalWrite(trigPin, LOW);
       duration = pulseIn(echoPin, HIGH);         
      //Calculate the distance (in cm) based on the speed of sound.
      distance = duration / 58.2;
      if(walk_1time == true){
        if(distance <= maximumRange && distance >= minimumRange) // ถ้าระยะน้อยกว่าค่าสูงสุด เเละมากกว่าค่าต่ำสุด
        { 
        
              Ser_work = true; //servoทำงาน
              light1_2_red = true;
              Serial.println(distance);  //แสดงค่าระยะทาง
              //digitalWrite(LEDPin, LOW);
              walk_Yellow = true;//ไฟเหลืองตอนเดิน
              shownum = true; //เลข9-0
              walk_1time = false;
              sensor_ON = false;
        }
      }
    
        
          if(shownum == true)
          {

            for(int k = 0;k<=200;k++) 
            {      
                      if(walk_Yellow == true)
                      {
                        if(light1_red == true)
                        {
                          digitalWrite(LIGHT_1_RED, LOW);
                          digitalWrite(LIGHT_1_YELLOW, HIGH); 
                          digitalWrite(LIGHT_1_GREEN, HIGH); 
                          digitalWrite(LIGHT_2_RED, HIGH);
                          digitalWrite(LIGHT_2_YELLOW, LOW); 
                          digitalWrite(LIGHT_2_GREEN, HIGH);
                        }
                        if(light2_red == true)
                        {
                          digitalWrite(LIGHT_1_RED, HIGH);
                          digitalWrite(LIGHT_1_YELLOW, LOW); 
                          digitalWrite(LIGHT_1_GREEN, HIGH); 
                          digitalWrite(LIGHT_2_RED, LOW);
                          digitalWrite(LIGHT_2_YELLOW, HIGH); 
                          digitalWrite(LIGHT_2_GREEN, HIGH);
                        }
                        
                      }
                       
                      if(count1_ON == true)
                      {
                        displayDigit(2);
                       displaySegment(d2);
                       delayMicroseconds(2500);                                       
                       displayDigit(1);
                       displaySegment(count);
                       delayMicroseconds(2500);                    
                      }
                      if(count2_ON == true)
                      {
                        displayDigit(4);
                       displaySegment2(d4);
                       delayMicroseconds(2500);                       
                       displayDigit(3);
                       displaySegment2(count2);
                       delayMicroseconds(2500);
                      }      
                      Serial.println(k); 
                                      
             }
             myservo.write(90); // สั่งให้ Servo หมุนไปองศาที่ 0
             myservo2.write(90); // สั่งให้ Servo หมุนไปองศาที่ 0
             walk_num = 9;
             set_check = check;
             walk_Yellow = false;
             walk_Red = true; // ไฟเเดงตอนเดิน
              count1_ON == false;
              count2_ON == false;
             walk_checklight1 = true;
             walk_checklight2 = true;
            //sensor_ON = true;
            
            shownum = false;
          }
          if(walk_Red == true) //ไฟเเดงตอนเดิน
          {
             myservo.write(90); // สั่งให้ Servo หมุนไปองศาที่ 0
             myservo2.write(90); // สั่งให้ Servo หมุนไปองศาที่ 0
             digitalWrite(buzzer_module, HIGH);
              check = 5;
              start_count1 = false;
              start_count2 = false;
                        digitalWrite(LIGHT_1_RED, LOW);
                        digitalWrite(LIGHT_1_YELLOW, HIGH); 
                        digitalWrite(LIGHT_1_GREEN, HIGH); 
                        digitalWrite(LIGHT_2_RED, LOW);
                        digitalWrite(LIGHT_2_YELLOW, HIGH); 
                        digitalWrite(LIGHT_2_GREEN, HIGH);
                        digitalWrite(LIGHT_Walk_GREEN, LOW);
                        digitalWrite(LIGHT_Walk_RED, HIGH);
                       
                      if(walk_checklight1 == true) //เเสดงเลขค้าง
                      {              
                        displayDigit(2);
                       displaySegment(d2);
                       delayMicroseconds(2500);                                       
                       displayDigit(1);
                       displaySegment(count);
                       delayMicroseconds(2500);                    
                      }
                      if(walk_checklight2 == true)//เเสดงเลขค้าง
                      {
                        displayDigit(4);
                       displaySegment2(d4);
                       delayMicroseconds(2500);                       
                       displayDigit(3);
                       displaySegment2(count2);
                       delayMicroseconds(2500);
                      }          
                      //Serial.println(l);
                      
                        displaySegment3(walk_num);
                        delayMicroseconds(5000);
                        walk_num--;
                        if(walk_num < 0)
                        {
//                          if(walk_num == 0)
//                          {
//                           Ser_work = false;
//                          }
                          for(int k = 0;k<=200;k++) 
                          {  
                            if(light1_red == true)
                            {
                              digitalWrite(LIGHT_1_RED, LOW);
                              digitalWrite(LIGHT_1_YELLOW, HIGH); 
                              digitalWrite(LIGHT_1_GREEN, HIGH); 
                              digitalWrite(LIGHT_2_RED, HIGH);
                              digitalWrite(LIGHT_2_YELLOW, LOW); 
                              digitalWrite(LIGHT_2_GREEN, HIGH);
                            }
                            if(light2_red == true)
                            {
                              digitalWrite(LIGHT_1_RED, HIGH);
                              digitalWrite(LIGHT_1_YELLOW, LOW); 
                              digitalWrite(LIGHT_1_GREEN, HIGH); 
                              digitalWrite(LIGHT_2_RED, LOW);
                              digitalWrite(LIGHT_2_YELLOW, HIGH); 
                              digitalWrite(LIGHT_2_GREEN, HIGH);
                            }
                            if(walk_checklight1 == true)
                            {              
                              displayDigit(2);
                             displaySegment(d2);
                             delayMicroseconds(2500);                                       
                             displayDigit(1);
                             displaySegment(count);
                             delayMicroseconds(2500);                    
                            }
                            if(walk_checklight2 == true)
                            {
                              displayDigit(4);
                             displaySegment2(d4);
                             delayMicroseconds(2500);                       
                             displayDigit(3);
                             displaySegment2(count2);
                             delayMicroseconds(2500);
                            }  
                          }
                          check = set_check;
                         
                         count1_ON = true;
                         count2_ON = true;
                         start_count1 = true;
                         start_count2 = true;
                         myservo.write(90); // สั่งให้ Servo หมุนไปองศาที่ 90
                         myservo2.write(90); // สั่งให้ Servo หมุนไปองศาที่ 90
                         
                         walk_Red = false;
                       }
                       
             }
             if(Ser_work == false)
             {
                 myservo.write(0); // สั่งให้ Servo หมุนไปองศาที่ 0
                 myservo2.write(0); // สั่งให้ Servo หมุนไปองศาที่ 0
                 digitalWrite(buzzer_module, LOW);       
             }
             
             
          
  }
  
void setup() 
{
  int status;
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) // non zero status means it was unsuccesful
  {
       // begin() failed so blink error code using the onboard LED if possible
       hd44780::fatalError(status); // does not return
  }
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(echoPin) ,interrupt_walk, CHANGE);
  pinMode(LIGHT_1_RED, OUTPUT);
  pinMode(LIGHT_1_YELLOW, OUTPUT);
  pinMode(LIGHT_1_GREEN, OUTPUT);

  pinMode(LIGHT_2_RED, OUTPUT);
  pinMode(LIGHT_2_YELLOW, OUTPUT);
  pinMode(LIGHT_2_GREEN, OUTPUT);

   pinMode(LIGHT_Walk_GREEN, OUTPUT);
   pinMode(LIGHT_Walk_RED, OUTPUT);
   
   pinMode(segmentA, OUTPUT);
   pinMode(segmentB, OUTPUT);
   pinMode(segmentC, OUTPUT);
   pinMode(segmentD, OUTPUT);
   pinMode(segmentE, OUTPUT);
   pinMode(segmentF, OUTPUT);
   pinMode(segmentG, OUTPUT);
   ///////////////////////////////////////////////////
   pinMode(segmentA2, OUTPUT);
   pinMode(segmentB2, OUTPUT);
   pinMode(segmentC2, OUTPUT);
   pinMode(segmentD2, OUTPUT);
   pinMode(segmentE2, OUTPUT);
   pinMode(segmentF2, OUTPUT);
   pinMode(segmentG2, OUTPUT);
   //////////////////////////////////////////////////
   pinMode(segmentA3, OUTPUT);
   pinMode(segmentB3, OUTPUT);
   pinMode(segmentC3, OUTPUT);
   pinMode(segmentD3, OUTPUT);
   pinMode(segmentE3, OUTPUT);
   pinMode(segmentF3, OUTPUT);
   pinMode(segmentG3, OUTPUT);
   //////////////////////////////////////////////////
   //pinMode(segmentDP, OUTPUT);
   pinMode(digit1, OUTPUT);
   pinMode(digit2, OUTPUT);
   pinMode(digit3, OUTPUT);
   pinMode(digit4, OUTPUT);



   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
   myservo.attach(10); 
   myservo2.attach(11); 
   pinMode(buzzer_module, OUTPUT);
 
    
    
}

// the loop function runs over and over again forever
void loop() 
{
  if(light1_red == true) //ไฟขวาเเดง ซ้ายเขียว ไม่มีการเดิน
  {
    lcd.clear();
    lcd.print("North:Red");
    lcd.setCursor(0, 1);
    lcd.print("South:Green");
    lcd.setCursor(12, 0);
    lcd.print("Walk");
    lcd.setCursor(13, 1);
    lcd.print("OFF");
    lcd.setCursor(0, 0);
  }
  if(light2_red == true) //ไฟขวาเขียว ซ้ายเเดง ไม่มีการเดิน
  {
    lcd.clear();
    lcd.print("North:Green");
    lcd.setCursor(0, 1);
    lcd.print("South:Red");
    lcd.setCursor(12, 0);
    lcd.print("Walk");
    lcd.setCursor(13, 1);
    lcd.print("OFF");
    lcd.setCursor(0, 0);
  }
  if(light1_2_red == true) //ไฟขวาเเดง ซ้ายเเดง มีการเดิน
  {
  
    lcd.clear();
    lcd.print("North:Red");
    lcd.setCursor(0, 1);
    lcd.print("South:Red");
    lcd.setCursor(12, 0);
    lcd.print("Walk");
    lcd.setCursor(13, 1);
    lcd.print("ON");
    lcd.setCursor(0, 0);
  }
  
  if(yellow1_ON == true) // เมื่อถึงเลข0 เเสดงไฟเหลือง2วิก่อนสลับไฟ
  {
       lcd.clear();
       check = 3;
       d2=3;
       count=0;
       d4=3;
       count2=0;
       if(check == 3)
       {
        digitalWrite(LIGHT_1_RED, HIGH);
        digitalWrite(LIGHT_1_YELLOW, LOW); 
        digitalWrite(LIGHT_1_GREEN, HIGH); 
        digitalWrite(LIGHT_2_RED, HIGH);
        digitalWrite(LIGHT_2_YELLOW, LOW); 
        digitalWrite(LIGHT_2_GREEN, HIGH);
        //Ser_work = false; 
       }     
       for(int i = 0; i <= 100;i++)
       {
        if(yellow1_ON == true)
        {
          displayDigit(1);
          displaySegment(yellow1_count);   
          delayMicroseconds(5000);
        }
        if(yellow2_ON == true)
        {
          displayDigit(3);
          displaySegment2(yellow1_count);
          delayMicroseconds(5000);
        }             
       }
       yellow1_count--;
       
       if(yellow1_count < 0) // คำสั่งสลับไฟ
       {
        check = set_check;
         if(check == 1)
         {
          check = 2;
         }
         else if(check == 2)
         {
          check = 1;
         }
         count1_ON = true;
         count2_ON = true;  
         yellow1_ON = false;
         yellow2_ON = false;
         
       }
       
  }
  
 //Main   
 if(check == 1)
 {
    digitalWrite(LIGHT_1_RED, LOW);
    digitalWrite(LIGHT_1_YELLOW, HIGH); 
    digitalWrite(LIGHT_1_GREEN, HIGH);

    digitalWrite(LIGHT_2_RED, HIGH);
    digitalWrite(LIGHT_2_YELLOW, HIGH); 
    digitalWrite(LIGHT_2_GREEN, LOW);

    digitalWrite(LIGHT_Walk_GREEN, HIGH);
    digitalWrite(LIGHT_Walk_RED, LOW);
    Ser_work = false;
    light1_red = true;
    light2_red = false;
    light1_2_red = false;
    walk_1time = true;
                      
                  
 }
 if(check == 2)
 {
    digitalWrite(LIGHT_1_RED, HIGH);
    digitalWrite(LIGHT_1_YELLOW, HIGH); 
    digitalWrite(LIGHT_1_GREEN, LOW);

    digitalWrite(LIGHT_2_RED, LOW);
    digitalWrite(LIGHT_2_YELLOW, HIGH); 
    digitalWrite(LIGHT_2_GREEN, HIGH);

    digitalWrite(LIGHT_Walk_GREEN, HIGH);
    digitalWrite(LIGHT_Walk_RED, LOW);
    Ser_work = false;
    light2_red = true;
    light1_red = false;
    light1_2_red = false;
    walk_1time = true;
 }
                                                                                  
 if(d2 < 0)
 {
                       set_check = check;
                       d2=0;
                       count=0;
                       yellow1_count = 2;
                       yellow2_count = 2;
                       yellow1_ON = true;
                       count1_ON = false;
 } 
                    
 if(d4 < 0)
 {
                      set_check = check;
                       d4=0;
                       count2=0;
                       yellow1_count = 2;
                       yellow2_count = 2;
                       yellow2_ON = true;
                       count2_ON = false;
 } 
 for(int j = 0;j<=100;j++) 
 {                     
    if(count1_ON == true)
    {
        displayDigit(2);
        displaySegment(d2);
        delayMicroseconds(2500);                     
                       
        displayDigit(1);
        displaySegment(count);
        delayMicroseconds(2500);                   
    }
    
    if(count2_ON == true)
    {
        displayDigit(4);
        displaySegment2(d4);
        delayMicroseconds(2500);
                       
                       
        displayDigit(3);
        displaySegment2(count2);
        delayMicroseconds(2500);
    }                                                                         
 }
                    
 interrupt_walk();
 if(count == 0) //เมื่อหลักหน่วยเป็น 0 ลดหลักสิบ
 {
     count = 10;
     d2--;
 }
 if(start_count1 == true) //นับถอยหลังจอซ้าย
 {
     count--;
     walk_Yellow = false;
     shownum = false;
 }
                    
 if(count2 == 0) //เมื่อหลักหน่วยเป็น 0 ลดหลักสิบ
 {
     count2 = 10;
     d4--;
 }
 if(start_count2 == true) //นับถอยหลังจอขวา
 {
     count2--;
     walk_Yellow = false;
     shownum = false;
 }
                        
}
///////////////////////////////////////////////////////////////////////////////
void displayDigit(int numberToDisplay)
{
  switch (numberToDisplay)
  {
    case 1:
      digitalWrite(digit1, HIGH);
      digitalWrite(digit2, LOW);
      digitalWrite(digit3, LOW);
      digitalWrite(digit4, LOW);
      break;
    case 2:
      digitalWrite(digit2, HIGH);
      digitalWrite(digit1, LOW);
      digitalWrite(digit3, LOW);
      digitalWrite(digit4, LOW);
      break;
    case 3:
      digitalWrite(digit3, HIGH);
      digitalWrite(digit1, LOW);
      digitalWrite(digit2, LOW);
      digitalWrite(digit4, LOW);
      break;
    case 4:
      digitalWrite(digit4, HIGH);
      digitalWrite(digit1, LOW);
      digitalWrite(digit3, LOW);
      digitalWrite(digit2, LOW);
      break;
      
    default:
      break;
  }
}
//////////////////////////////////////////////////////////////////////////////
void displaySegment(int numberToDisplay)
{
 switch (numberToDisplay)
 {
  case 0: 
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentE, HIGH);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, LOW);
      break;
 case 1: 
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, LOW);
      break;
    case 2: 
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, LOW);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentE, HIGH);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, HIGH);
      break;
    case 3: 
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, HIGH);
      break;
    case 4: 
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, HIGH);
      break;
    case 5: 
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, HIGH);
      break;
    case 6: 
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentE, HIGH);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, HIGH);
      break;
    case 7: 
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, LOW);
      break;
    case 8: 
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, HIGH);
      digitalWrite(segmentE, HIGH);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, HIGH);
      break;
    case 9: 
      digitalWrite(segmentA, HIGH);
      digitalWrite(segmentB, HIGH);
      digitalWrite(segmentC, HIGH);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, HIGH);
      digitalWrite(segmentG, HIGH);
      break;
    
 }
}
//////////////////////////////////
void displaySegment2(int numberToDisplay)
{
 switch (numberToDisplay)
 {
  case 0: 
      digitalWrite(segmentA2, HIGH);
      digitalWrite(segmentB2, HIGH);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, HIGH);
      digitalWrite(segmentE2, HIGH);
      digitalWrite(segmentF2, HIGH);
      digitalWrite(segmentG2, LOW);
      break;
 case 1: 
      digitalWrite(segmentA2, LOW);
      digitalWrite(segmentB2, HIGH);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, LOW);
      digitalWrite(segmentE2, LOW);
      digitalWrite(segmentF2, LOW);
      digitalWrite(segmentG2, LOW);
      break;
    case 2: 
      digitalWrite(segmentA2, HIGH);
      digitalWrite(segmentB2, HIGH);
      digitalWrite(segmentC2, LOW);
      digitalWrite(segmentD2, HIGH);
      digitalWrite(segmentE2, HIGH);
      digitalWrite(segmentF2, LOW);
      digitalWrite(segmentG2, HIGH);
      break;
    case 3: 
      digitalWrite(segmentA2, HIGH);
      digitalWrite(segmentB2, HIGH);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, HIGH);
      digitalWrite(segmentE2, LOW);
      digitalWrite(segmentF2, LOW);
      digitalWrite(segmentG2, HIGH);
      break;
    case 4: 
      digitalWrite(segmentA2, LOW);
      digitalWrite(segmentB2, HIGH);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, LOW);
      digitalWrite(segmentE2, LOW);
      digitalWrite(segmentF2, HIGH);
      digitalWrite(segmentG2, HIGH);
      break;
    case 5: 
      digitalWrite(segmentA2, HIGH);
      digitalWrite(segmentB2, LOW);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, HIGH);
      digitalWrite(segmentE2, LOW);
      digitalWrite(segmentF2, HIGH);
      digitalWrite(segmentG2, HIGH);
      break;
    case 6: 
      digitalWrite(segmentA2, HIGH);
      digitalWrite(segmentB2, LOW);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, HIGH);
      digitalWrite(segmentE2, HIGH);
      digitalWrite(segmentF2, HIGH);
      digitalWrite(segmentG2, HIGH);
      break;
    case 7: 
      digitalWrite(segmentA2, HIGH);
      digitalWrite(segmentB2, HIGH);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, LOW);
      digitalWrite(segmentE2, LOW);
      digitalWrite(segmentF2, LOW);
      digitalWrite(segmentG2, LOW);
      break;
    case 8: 
      digitalWrite(segmentA2, HIGH);
      digitalWrite(segmentB2, HIGH);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, HIGH);
      digitalWrite(segmentE2, HIGH);
      digitalWrite(segmentF2, HIGH);
      digitalWrite(segmentG2, HIGH);
      break;
    case 9: 
      digitalWrite(segmentA2, HIGH);
      digitalWrite(segmentB2, HIGH);
      digitalWrite(segmentC2, HIGH);
      digitalWrite(segmentD2, LOW);
      digitalWrite(segmentE2, LOW);
      digitalWrite(segmentF2, HIGH);
      digitalWrite(segmentG2, HIGH);
      break;
    
 }
}

void displaySegment3(int numberToDisplay)
{
 switch (numberToDisplay)
 {
  case 0: 
      digitalWrite(segmentA3, HIGH);
      digitalWrite(segmentB3, HIGH);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, HIGH);
      digitalWrite(segmentE3, HIGH);
      digitalWrite(segmentF3, HIGH);
      digitalWrite(segmentG3, LOW);
      break;
 case 1: 
      digitalWrite(segmentA3, LOW);
      digitalWrite(segmentB3, HIGH);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, LOW);
      digitalWrite(segmentE3, LOW);
      digitalWrite(segmentF3, LOW);
      digitalWrite(segmentG3, LOW);
      break;
    case 2: 
      digitalWrite(segmentA3, HIGH);
      digitalWrite(segmentB3, HIGH);
      digitalWrite(segmentC3, LOW);
      digitalWrite(segmentD3, HIGH);
      digitalWrite(segmentE3, HIGH);
      digitalWrite(segmentF3, LOW);
      digitalWrite(segmentG3, HIGH);
      break;
    case 3: 
      digitalWrite(segmentA3, HIGH);
      digitalWrite(segmentB3, HIGH);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, HIGH);
      digitalWrite(segmentE3, LOW);
      digitalWrite(segmentF3, LOW);
      digitalWrite(segmentG3, HIGH);
      break;
    case 4: 
      digitalWrite(segmentA3, LOW);
      digitalWrite(segmentB3, HIGH);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, LOW);
      digitalWrite(segmentE3, LOW);
      digitalWrite(segmentF3, HIGH);
      digitalWrite(segmentG3, HIGH);
      break;
    case 5: 
      digitalWrite(segmentA3, HIGH);
      digitalWrite(segmentB3, LOW);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, HIGH);
      digitalWrite(segmentE3, LOW);
      digitalWrite(segmentF3, HIGH);
      digitalWrite(segmentG3, HIGH);
      break;
    case 6: 
      digitalWrite(segmentA3, HIGH);
      digitalWrite(segmentB3, LOW);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, HIGH);
      digitalWrite(segmentE3, HIGH);
      digitalWrite(segmentF3, HIGH);
      digitalWrite(segmentG3, HIGH);
      break;
    case 7: 
      digitalWrite(segmentA3, HIGH);
      digitalWrite(segmentB3, HIGH);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, LOW);
      digitalWrite(segmentE3, LOW);
      digitalWrite(segmentF3, LOW);
      digitalWrite(segmentG3, LOW);
      break;
    case 8: 
      digitalWrite(segmentA3, HIGH);
      digitalWrite(segmentB3, HIGH);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, HIGH);
      digitalWrite(segmentE3, HIGH);
      digitalWrite(segmentF3, HIGH);
      digitalWrite(segmentG3, HIGH);
      break;
    case 9: 
      digitalWrite(segmentA3, HIGH);
      digitalWrite(segmentB3, HIGH);
      digitalWrite(segmentC3, HIGH);
      digitalWrite(segmentD3, LOW);
      digitalWrite(segmentE3, LOW);
      digitalWrite(segmentF3, HIGH);
      digitalWrite(segmentG3, HIGH);
      break;
    
 }
}
