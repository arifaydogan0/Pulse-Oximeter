#include <main.h>
#include <math.h>
#fuses INTRC_IO,NOWDT,NOPUT,NOPROTECT,NOCPD  //Dahili osilat�r aktif
#use delay(clock=4000000)
#use fast_io(a)
#use fast_io(b)

int16 i=0;

#int_timer1
void timer1kesme()   //i=500 olana kadar i her kesmede bir artacak.
{
 set_timer1(60536);
  if(i<500)          //i=500 oldugunda 20 saniyede 500 kesme oluscak. 
  {i++;}
}

void main()
{
int pulse;
int nabiz;
int ekran1=0;
int ekran2=0;
int ekran3=0;
int a=0;
int b=0;

set_tris_a(0x34);
set_tris_b(0x80);

output_a(0x00);
output_b(0x00);

enable_interrupts(int_timer1);
enable_interrupts(GLOBAL);

   while(TRUE)
   {  
      output_a(0x00);
      output_b(0x00);
       
      if(input(pin_a4))     //E�er start butonuna bas�l�rsa..
      {
        pulse=0;
        nabiz=0;
        
        delay_ms(3000);
        output_high(pin_b6);       //Ekranlar�n orta �ubu�u a��ld�(yan-s�n i�in)
        output_high(pin_a3);       //Ledi yak
        
        setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);   //B�lme oran� 2, dahili osilat�r.
        set_timer1(60536);                          //Saymaya ba�lama de�eri. 5k*8=40k yani 40ms de bir kesme oluscak
           
           while(i<500)                //i=500 olmad�kca �lc�me devam et(i=500 olunca 20 saniye dolacak)
           {
             if(input(pin_a2))       //Sinyal gelirse..
             {
               delay_ms(222);
               output_high(pin_a0);   //Ekranar�n transist�rleri a��l�yor
               output_high(pin_a6);
               output_high(pin_a7);
               output_high(pin_a1);  //Buzzer �ts�n
               delay_ms(77);  
               pulse++;              //Pulseyi bir artt�r       
             }
            output_low(pin_a1);     //Ekranlar�n transist�rleri kapan�yor(�l��m esnas�nda yan-s�n yap�yolar.)
            output_low(pin_a0);
            output_low(pin_a6);
            output_low(pin_a7);
           }
        setup_timer_1(T1_DISABLED);     //Timer1 kapat
        i=0;
        output_low(pin_a3);            //Ledi s�nd�r(yanmas�-s�nmesi aras� zaman 10 saniye olmal�)
        
        nabiz=pulse*3;               //20 saniyedeki pulse say�s�ndan bir dakikadaki nab�z hesaplan�yor.
        
        ekran3=nabiz%10;        //Nab�z say�s� kullan�larak �� ekrana yazd�r�lacak rakamlar hesaplan�yor
        
        a=(nabiz-ekran3)/10;
        ekran2=a%10;
        
        b=(nabiz-((ekran2*10)+ekran3))/100;
        ekran1=b%10;
        
      int dizi[10]={63,3,109,103,83,118,126,35,127,119};     //0-9 aras� rakamlar� yazd�rmak i�in binary kodlardan dizi olu�turuldu

      for(int16 display=0; display<10000; display++)    //90 saniye boyunca nabiz ekranda g�sterilecek
      {
        output_b(dizi[ekran3]);
        output_high(pin_a6);
        delay_ms(3);
        output_low(pin_a6);
        
        output_b(dizi[ekran2]);
        output_high(pin_a7);
        delay_ms(3);
        output_low(pin_a7);
        
        output_b(dizi[ekran1]);
        output_high(pin_a0);
        delay_ms(3);
        output_low(pin_a0);
      }
        
        
      }

   }

}
   
