#include <main.h>
#include <math.h>
#fuses INTRC_IO,NOWDT,NOPUT,NOPROTECT,NOCPD  //Dahili osilatör aktif
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
       
      if(input(pin_a4))     //Eðer start butonuna basýlýrsa..
      {
        pulse=0;
        nabiz=0;
        
        delay_ms(3000);
        output_high(pin_b6);       //Ekranlarýn orta çubuðu açýldý(yan-sön için)
        output_high(pin_a3);       //Ledi yak
        
        setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);   //Bölme oraný 2, dahili osilatör.
        set_timer1(60536);                          //Saymaya baþlama deðeri. 5k*8=40k yani 40ms de bir kesme oluscak
           
           while(i<500)                //i=500 olmadýkca ölcüme devam et(i=500 olunca 20 saniye dolacak)
           {
             if(input(pin_a2))       //Sinyal gelirse..
             {
               delay_ms(222);
               output_high(pin_a0);   //Ekranarýn transistörleri açýlýyor
               output_high(pin_a6);
               output_high(pin_a7);
               output_high(pin_a1);  //Buzzer ötsün
               delay_ms(77);  
               pulse++;              //Pulseyi bir arttýr       
             }
            output_low(pin_a1);     //Ekranlarýn transistörleri kapanýyor(Ölçüm esnasýnda yan-sön yapýyolar.)
            output_low(pin_a0);
            output_low(pin_a6);
            output_low(pin_a7);
           }
        setup_timer_1(T1_DISABLED);     //Timer1 kapat
        i=0;
        output_low(pin_a3);            //Ledi söndür(yanmasý-sönmesi arasý zaman 10 saniye olmalý)
        
        nabiz=pulse*3;               //20 saniyedeki pulse sayýsýndan bir dakikadaki nabýz hesaplanýyor.
        
        ekran3=nabiz%10;        //Nabýz sayýsý kullanýlarak üç ekrana yazdýrýlacak rakamlar hesaplanýyor
        
        a=(nabiz-ekran3)/10;
        ekran2=a%10;
        
        b=(nabiz-((ekran2*10)+ekran3))/100;
        ekran1=b%10;
        
      int dizi[10]={63,3,109,103,83,118,126,35,127,119};     //0-9 arasý rakamlarý yazdýrmak için binary kodlardan dizi oluþturuldu

      for(int16 display=0; display<10000; display++)    //90 saniye boyunca nabiz ekranda gösterilecek
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
   
