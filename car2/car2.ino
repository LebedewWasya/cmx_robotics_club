#include <SoftwareSerial.h>// либа для блютуза
#include "AFMotor.h" // ллиба для машины

AF_DCMotor leftMotor(1); //
AF_DCMotor rightMotor(2); //

// верно для мотор шилда, для самой ардуино + 1 на номера п0ртов
// RX - цифровой вывод 9 (необходимо соединить с выводом TX другого устройства)
// TX - цифровой вывод 10 (необходимо соединить с выводом RX другого устройства)
SoftwareSerial bluetoothSerial(9, 10);

char bluetoothData[7] = {}; // полностью считанная стока.
int routePart = 0; //число от 0 до 359
int speedPart = 0; //число от 0 до 1000 (215 умножается на x\1000)
int speedDefoult = 40; 
int speedLeftMotor = 0;
int speedRightMotor = 0;

void setup() {
  Serial.begin(9600); // скорость отправки на комп сообщений (для отладки)
  bluetoothSerial.begin(9600); // скорость считывания данных с блютуза
  Serial.println("started");
  putin();
}

void loop() {
//  readIntegerFromBluetooth();
leftMotor.run(FORWARD);
rightMotor.run(FORWARD);
leftMotor.setSpeed(255);
rightMotor.setSpeed(255);

//  routePart = charToInt(bluetoothData[0]) * 100 + charToInt(bluetoothData[1]) * 10 + charToInt(bluetoothData[2]);
//  speedPart = charToInt(bluetoothData[3])* 1000 + charToInt(bluetoothData[4]) * 100 + charToInt(bluetoothData[5]) * 10 + charToInt(bluetoothData[6]);

//  route();
  //  Serial.println(bluetoothData);
  delay(2000);
  //  Serial.println("end");
}

// Управление -------------------------------------------------------------------------------------------

void route() {
  calcSpeed();
  choiceCirclePart();
}

void calcSpeed() {
  speedDefoult = 40 + (int)( ((float)speedPart / (float)1000) * (float)215 );
  speedLeftMotor = speedDefoult;
  speedRightMotor = speedDefoult;
}

void choiceCirclePart() {
  if (routePart <= 90) {
    circlePartZero();
  } else {
    if (routePart <= 180) {
      circlePartOne();
    } else {
      if (routePart <= 270) {
        circlePartTwo();
      } else {
        if (routePart <= 359) {
          circlePartThree();
        }
      }
    }
  }
  
  //Serial.print("скорость моотора 1 ");
  //vivodIntVsosnole(speedLeftMotor);
  //Serial.println("");
  leftMotor.setSpeed(speedLeftMotor);
  
  //Serial.print("скорость моотора 2 ");
  //vivodIntVsosnole(speedRightMotor);
  //Serial.println("");
  rightMotor.setSpeed(speedRightMotor);
}

void circlePartZero() {
  leftMotor.run(FORWARD);
  //Serial.println("мотор 1 FORWARD");
  rightMotor.run(FORWARD);
  //Serial.println("мотор 2 FORWARD");

  routePart = routePart / 10;
  if (routePart == 9) {routePart = 8;}
  speedRightMotor = (int)( ((float)routePart / (float)8) * (float)speedRightMotor );
}

void circlePartOne() {
  leftMotor.run(FORWARD);
  //Serial.println("мотор 1 FORWARD");
  rightMotor.run(FORWARD);
  //Serial.println("мотор 2 FORWARD");

  routePart = (180 - routePart) / 10;
  speedLeftMotor = (int)( ((float)(routePart) / (float)8) * (float)speedRightMotor );
}

void circlePartTwo() {
  leftMotor.run(BACKWARD);
  //Serial.println("мотор 1 BACKWARD");
  rightMotor.run(BACKWARD);
  //Serial.println("мотор 1 BACKWARD");

  routePart = (routePart - 180) / 10;
  if (routePart == 9) {routePart = 8;}
  speedLeftMotor = (int)( ((float)(routePart) / (float)8) * (float)speedRightMotor );
}

void circlePartThree() {
  leftMotor.run(BACKWARD);
  //Serial.println("мотор 1 BACKWARD");
  rightMotor.run(BACKWARD);
  //Serial.println("мотор 1 BACKWARD");

  routePart = (360 - routePart) / 10;
  speedRightMotor = (int)( ((float)(routePart) / (float)8) * (float)speedRightMotor );
}

// Все что связано со считыванием -----------------------------------------------------------------------
// метод для считывания 7-ми значного числа с блютуза
// если блюттуз отвалилися - 0 00 0000 - машина должна остановиться

void readIntegerFromBluetooth() {
  if (bluetoothSerial.available()) {
    int i = 0;
    int p = 0;
    
    int bluetoothDataElement = bluetoothSerial.read();

    while ((bluetoothDataElement != '#') && (p != 100)) {
      if (bluetoothSerial.available()) {
        bluetoothData[i] = bluetoothDataElement;
        Serial.print(bluetoothData[i]);
        
        ++i;
        bluetoothDataElement = bluetoothSerial.read();
      } else {
        // защита от вечного цикла
        ++p;
      }  
    }
 //   Serial.println();
  } else {
    putin();      
  }
}

// ну типо путин - обнуление))0
void putin() {
  for (int j = 0; j < 7; j++ ) {
    bluetoothData[j] = '0';
  }
}

// Утилиты -----------------------------------------------------------
int charToInt(char ch) {
  return ch - '0';
}

// выводит в консоль числа мать их
void vivodIntVsosnole (int chisloy) {

  int ciferka = chisloy % 10;
  chisloy = chisloy / 10;

  if (chisloy > 0) {
    vivodIntVsosnole(chisloy);
  }

  char ch  = ciferka + '0';
  Serial.print(ch);
}
