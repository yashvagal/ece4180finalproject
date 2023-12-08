#include "mbed.h"
#include "rtos.h"
#include "wave_player.h"
#include "SDFileSystem.h"
#include "Motor.h"
//#include "RGBLed.h"
#include "string"
#include "uLCD_4DGL.h"

Motor motorA(p26, p21, p22); // pwm, fwd, rev
Motor motorB(p25, p24, p23); // pwm, fwd, rev

// Setup RGB led using PWM pins and class
//RGBLed myRGBled(p22,p23,p24); //red, green, blue PWM pins

// Setup to play wav file from SD Card
AnalogOut DACout(p18);
wave_player waver(&DACout);
SDFileSystem sd(p5, p6, p7, p8, "sd"); //SD card
uLCD_4DGL LCD(p9,p10,p11);
Mutex lcd_mutex;

// Setup for bluetooth
Serial blue(p28,p27);
BusOut myled(LED1,LED2,LED3,LED4);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
volatile int songid=0;
volatile bool play=false;
string path;

//also setting any unused analog input pins to digital outputs reduces A/D noise a bit
//see http://mbed.org/users/chris/notebook/Getting-best-ADC-performance/
//DigitalOut P16(p16);
//DigitalOut P17(p17);
//DigitalOut P19(p19);
//DigitalOut P20(p20);
    char bnum = 0;
    char bhit = 0;
float motorSpeed = 0.0;
string song_title[4] = {"bsdop5", "gintamaop17", "drstoneop4", "narutoshippudenop19"};

void thread1(void const *args)
{
    // need to change code to use threads to play the wav file while avoiding disrupting continuous code flow
    // open wav file
    FILE *wave_file;
    while(1)
    {
        if(play==true)
    {
        led3 = !led3;
            path = "/sd/";
            path += song_title[songid];
            path += ".wav";
            wave_file=fopen(path.c_str(),"r");
            waver.play(wave_file);
            fclose(wave_file);
            play=false;
        Thread::wait(1000);

        }
    }
    
    
}

/*
void thread3(void const *args)
{
    int i;
    while(true) 
    {       // thread loop
        lcd_mutex.lock();
        LCD.locate(0,0);
        //LCD.set_font((unsigned char*) Small_6);
        LCD.printf("Thread2 count: %d",i);
        lcd_mutex.unlock();
        i++;
        Thread::wait(1000);
    }
}
*/
/*
void thread3(void const *args)
{
    // need to change code to use threads to play the wav file while avoiding disrupting continuous code flow
    // open wav file
    //FILE *wave_file;
    while(1)
    {
        int i=0;
        if((play==true)&&(i==0))
        {
            lcd_mutex.lock();
            LCD.locate(0,5);
            lcd_mutex.unlock();
            LCD.printf(song_title[songid].c_str());
            Thread::wait(10);
            i++;
        }
    }
}
*/



void thread2(void const *args)
{
    // need to change code to use threads to play the wav file while avoiding disrupting continuous code flow
    // open wav file
    while(1)
    {    
    motorA.speed(motorSpeed);
    motorB.speed(motorSpeed);
        if (blue.getc() == '!') 
        {
            if (blue.getc() == 'B') 
            { //button data packet
                bnum = blue.getc(); //button number
                bhit = blue.getc(); //1=hit, 0=release
                if (blue.getc() == char(~('!' + 'B' + bnum + bhit))) 
                { //checksum OK?
                   // myled = bnum - '1'; //current button number will appear on LEDs
                  led4 = !led4;

                    switch (bnum) 
                    {
                        case '1': //number button 1
                            if (bhit=='1') 
                            {
                                //add hit code here
                                songid = bnum -'1';
                                play=true;
                                LCD.printf(song_title[songid].c_str());
                                led1 = !led1;
    //                            LCD.printf(song_title[songid].c_str());

                                //motorSpeed += 0.2; // speed up
                                //myRGBled.write(0.0,1.0,0.0); //green
                            } 
                            else 
                            {
                                //add release code here
                            }
                            break;
                        case '2': //number button 2
                            if (bhit=='1') 
                            {
                            songid = bnum -'1';
                            play=true;
                            LCD.printf(song_title[songid].c_str());
                            led1 = !led1;
                            } 
                            break;
                        case '3': //number button 3
                            if (bhit=='1') 
                            {
                                songid = bnum -'1';
                                play=true;
                                LCD.printf(song_title[songid].c_str());
                                led1 = !led1;
    //add hit code here
                            } 
                            
                            break;
                        case '4': //number button 4
                            if (bhit=='1') 
                            {
                                songid = bnum -'1';
                                play=true;
                                LCD.printf(song_title[songid].c_str());
                                led1 = !led1;
                            }
                            
                            break;
                        case '5': //button 5 up arrow
                            if (bhit=='1') 
                            {
                                led2 = !led2;
                                //add hit code here
                                motorA.speed(1.0);
                                motorB.speed(1.0);
                                Thread::wait(100);
                                motorA.speed(0.0);
                                motorB.speed(0.0);
                                 // speed up
                                //myRGBled.write(0.0,1.0,1.0); // teal
                            } 
                            break;
                        case '6': //button 6 down arrow
                            if (bhit=='1') 
                            {
                                led2 = !led2;

                                motorA.speed(-1.0);
                                motorB.speed(-1.0);
                                Thread::wait(100);
                                motorA.speed(0.0);
                                motorB.speed(0.0);
                            } 
                            break;
                        case '7': //button 7 left arrow
                            if (bhit=='1') 
                            {
                                led2 = !led2;
                                //add hit code here
                                //myRGBled.write(0.3,0.0,0.6); // purple
                                motorA.speed(-1.0);
                                motorB.speed(1.0);
                                Thread::wait(100);
                                motorA.speed(0.0);
                                motorB.speed(0.0);
                            } 
                            
                            break;
                        case '8': //button 8 right arrow
                            if (bhit=='1') 
                            {
                                led2 = !led2;
                                motorA.speed(1.0);
                                motorB.speed(-1.0);
                                Thread::wait(100);
                                motorA.speed(0.0);
                                motorB.speed(0.0);
                            } 
                            
                            break;
                        default:
                            break;
                    }
                }
            }
     
        }   
    }
}

/*void thread3(void const *args)
{
    int i;
    while(true) 
    {       // thread loop
        //lcd_mutex.lock();
        LCD.locate(0,0);
        //LCD.set_font((unsigned char*) Small_6);
        LCD.printf("Thread2 count: %d",i);
        lcd_mutex.unlock();
        i++;
        Thread::wait(100);
    }
}
*/





int main()
{
  LCD.cls();

    LCD.text_height(1);
    LCD.text_width(1);
    LCD.color(WHITE);
    //LCD.printf("Temp Disp");
    


    
Thread t1(thread1);
Thread t2(thread2);


//Thread t3(thread3);

    while(1) 
    {
        Thread::wait(50);
    }

}
