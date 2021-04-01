/*
 Name:    ULVI_Dezen.ino
 Created: 23.02.2021 21:53:45
*/

#include <AvrI2c_Greiman.h>
#include <Servo.h>  //<Servo kütüphanesi
//#include <Wire.h>
#include <LiquidCrystal_I2C_AvrI2C.h>
LiquidCrystal_I2C_AvrI2C lcd(0x3F, 16, 2);

Servo servo;
int trigPin = 9;  // trigger pin
int echoPin = 8;  // echo pin
int anahtar =5; // Kapak switch i
//#define anahtar 5 // Kapak switch i
#define lamba 4 // uvc
long sayi = 0, lastTime1, lastTime2, uzaklik, g = 0;
int sure, hsure, temizliksuresi, s = 0;
bool say = false;
unsigned long milisdegeri, tsuresi = 0;



void setup()
{
    pinMode(trigPin, OUTPUT);
    pinMode(lamba, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(anahtar, INPUT);
    servo.attach(6);  // servo pini 
    servo.write(0);   // servoyu 0 dereceye götür
    Serial.begin(9600);

    // LCD tanımlamaları
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(2, 0); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
    lcd.print("ULVI - Dezen");//dilediğiniz ismi yazabilirsiniz. 
    lcd.setCursor(4, 1); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
    lcd.print("Hazir...");//dilediğiniz ismi yazabilirsiniz. 

    temizliksuresi = 2;  //yazılan sayı dakikadır.
}



void loop()
{

    hsure = temizliksuresi;
    milisdegeri = millis();
    //servo işlemleri

    sensorkontrol();
    guvenlikZaman();

//    Serial.print("uzaklık ");
//    Serial.print(uzaklik);
//    Serial.print("  el ");
//    Serial.print(sayi);
//    Serial.print("  basşladı ");
//    Serial.print(say);
    //Serial.print(" // ");
    

Serial.print(digitalRead(anahtar));

    delay(1000);
}


void sensorkontrol()
{

    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    sure = pulseIn(echoPin, HIGH);
    uzaklik = sure / 29.1 / 2;
    // uzaklık yazan değer sensörden okunan değer         
    if (uzaklik < 50)  // buradaki 200 duruma göre değiştirmeniz gereken değer
    {
        sayi++;
    }
    if (sayi == 1)
    {
        if (milisdegeri - lastTime2 > 2000)
        {
            //lcd.clear(0,0);
            servo.write(155);
            lcd.setCursor(0, 0); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
            say = false;
            lcd.clear();
            digitalWrite(lamba,1);
            lcd.print("Kapak Acik     ");//dilediğiniz ismi yazabilirsiniz. 
            lcd.setCursor(0, 1);
            lcd.print("Malzeme Koyunuz!");//dilediğiniz ismi yazabilirsiniz.
            lastTime1 = milisdegeri;
        }


        // delay(2000); 
    }
    if (sayi > 1)
    {
        // milisdegeri = millis();
        if (milisdegeri - lastTime1 > 2000)
        {
            servo.write(0);
            say = true;
            lcd.clear();
            lcd.setCursor(0, 0); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
            lcd.print("Kapak Kapali   ");//dilediğiniz ismi yazabilirsiniz.  
            sayi = 0;
            lastTime2 = milisdegeri;
            s = 0;
            // temizliksuresi = milisdegeri;                  
            tsuresi = milisdegeri;
            

        }

        // delay(2000); 
    }

}


void guvenlikZaman()
{

    if (say == true)
    {
        if (digitalRead(anahtar) == 1)   //anahtar basılı iken sıfır gelir
        {
            digitalWrite(lamba, 0);
            Serial.print("UVC açık //");


        }
        else
        {
            digitalWrite(lamba, 1);
            Serial.print("UVC kapalı //");
            say = false;
        }


        if (s < hsure)
        {
            if (milisdegeri - tsuresi < 60000) {
                lcd.clear();
                lcd.setCursor(0, 0); // İmleci sütun 0'a, satır 0'ye ayarlıyoruz
                lcd.print("Kapak Kapali   ");
                lcd.setCursor(2, 1); // İmleci sütun 1'a, satır 2'ye ayarlıyoruz
                lcd.print(":");
                lcd.setCursor(3, 1); // İmleci sütun 3'a, satır 2'ye ayarlıyoruz
                lcd.print(milisdegeri - tsuresi);

                if ((milisdegeri - tsuresi) < 10000)
                {
                    lcd.setCursor(4, 1);
                    lcd.print("     ");
                }
                else
                {
                    lcd.setCursor(5, 1);
                    lcd.print("    ");
                }

                if (s < 10) {
                    lcd.setCursor(0, 1); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
                    lcd.print(0);
                    lcd.setCursor(1, 1); // İmleci sütun 1'a, satır 2'ye ayarlıyoruz
                    lcd.print(s);
                }
                else {
                    lcd.setCursor(0, 1); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
                    lcd.print(s);
                }
                lcd.setCursor(8, 1);
                lcd.print("UVC ACIK");

            }
            else {
                s++;
                // digitalWrite(lamba,0);
                       //say=false;
                lcd.clear();
                lcd.setCursor(0, 0); // İmleci sütun 0'a, satır 1'ye ayarlıyoruz
                lcd.print("Kapak Kapali   ");

                if (s < 10) {
                    lcd.setCursor(0, 1); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
                    lcd.print(0);
                    lcd.setCursor(1, 1); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
                    lcd.print(s);
                }
                else {
                    lcd.setCursor(0, 1); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
                    lcd.print(s);
                }
                lcd.setCursor(8, 1);
                lcd.print("UVC ACIK");
                tsuresi = milisdegeri;
            }

        }
        else {
            digitalWrite(lamba,0);     
            lcd.clear();
            lcd.setCursor(0, 0); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
            lcd.print("Kapak Kapali   ");//dilediğiniz ismi yazabilirsiniz. 
            lcd.setCursor(0, 1); // İmleci sütun 0'a, satır 2'ye ayarlıyoruz
            lcd.print("Islem Tamamlandi");//dilediğiniz ismi yazabilirsiniz.
            say = false;
        }



    }
}
