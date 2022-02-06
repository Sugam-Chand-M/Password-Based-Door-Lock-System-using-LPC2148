#include<lpc214x.h>
#define bit(x) (1<<x)
void delay ()
{
 unsigned int temp, ct;
 for(ct=0; ct<30; ct++) {
 for(temp=0; temp < 65000; temp++);
 }
}
unsigned int range=0,i;
/* --------- DC Motor ----- */
void forward(void);
void reverse(void);
void stop(void);
/* --------- LCD ---------*/
void lcd_init(void);
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay(void);
/* ---------- Keypad ------*/
#define c1 (IOPIN1&1<<20)
#define c2 (IOPIN1&1<<21)
#define c3 (IOPIN1&1<<22)
unsigned char r_loc,c_loc;
unsigned char keypad(void);
/* ------- Main -------*/
int main()
{ 
char a, b, c, d; 
 VPBDIV=0x01; // PCLK = 60MHz
 IO1DIR |= 0x0f<<16;
 IO0DIR |= 0xf00fc;
 
 lcd_init();
 while(1) {
 cmd(0x80);
 show("#Enter Password#");
 
 cmd(0xc5);
a = keypad();
dat('*');
b = keypad();
dat('*');
c = keypad();
dat('*');
d = keypad();
dat('*');
if(('5' == a) && ('5' == b) && ('5' == c) && ('5' == d) ) {
 cmd(0xc0);
 show(" Thank You! ");
 forward();
 delay();
 stop();
 cmd(0xc0);
 show(" Come Again!! ");
 delay();
 reverse();
 delay();
 stop();
 } else {
 cmd(0xc0);
 show("~Wrong Password~");
 delay();
 }
 cmd(0x01); 
 }
}
/* ----- Keypad Function ----*/
unsigned char keypad()
{
 IO1PIN &= ~(0xff<<16);
 IO1PIN |= 0xf0<<16;
 while(c1 && c2 && c3);
 while(!c1 || !c2 || !c3) {
if(!c1 && c2 && c3) c_loc=0;
 else if(c1 && !c2 && c3) c_loc=1;
 else if(c1 && c2 && !c3) c_loc=2;
 IO1CLR = 1<<16;
 IO1SET = 0x0e<<16;
 if(!c1 || !c2 || !c3) {
 r_loc=0;
 break;
 }
 IO1CLR = 1<<17;
 IO1SET = 0x0d<<16;
 if(!c1 || !c2 || !c3) {
 r_loc=1;
 break;
 }
 
 IO1CLR = 1<<18;
 IO1SET = 0x0b<<16;
 if(!c1 || !c2 || !c3) {
 r_loc=2;
 break;
 }
 IO1CLR = 1<<19;
 IO1SET = 0x07<<16;
 if(!c1 || !c2 || !c3) {
 r_loc=3;
break;
 }
 }
 while(!c1 || !c2 || !c3);
 switch(c_loc){
case 0: switch(r_loc){
case 0: return '1';
case 1: return '4';
case 2: return '7';
case 4: return '*';
}
break;
case 1: switch(r_loc){
case 0: return '2';
case 1: return '5';
case 2: return '8';
case 4: return '0';
}
break;
case 2: switch(r_loc){
case 0: return '3';
case 1: return '6';
case 2: return '9';
case 4: return '#';
}
break;
}
return '\0';
}
/* ----- LCD Functions -------*/
void lcd_init()
{
 cmd(0x02);
 cmd(0x28);
 cmd(0x0c);
 cmd(0x06);
 cmd(0x80);
}
void cmd(unsigned char a)
{
 IO0PIN &= 0xffffff03;
 IO0PIN |= (a & 0xf0) << 0;
 IO0CLR |= bit(2); //rs=0
 IO0CLR |= bit(1); //rw=0
 IO0SET |= bit(3); //en=1
 lcd_delay();
 IO0CLR |= bit(3); //en=0
 
 IO0PIN &= 0xffffff03;
 IO0PIN |= ((a << 4) & 0xf0) << 0;
 IO0CLR |= bit(2); //rs=0
 IO0CLR |= bit(1); //rw=0
 IO0SET |= bit(3); //en=1
 lcd_delay();
IO0CLR |= bit(3); //en=0
}
void dat(unsigned char b)
{
 IO0PIN &= 0xffffff03;
 IO0PIN |= (b & 0xf0) << 0;
 IO0SET |= bit(2); //rs=1
 IO0CLR |= bit(1); //rw=0
 IO0SET |= bit(3); //en=1
 lcd_delay();
 IO0CLR |= bit(3); //en=0
 
 IO0PIN &= 0xffffff03;
 IO0PIN |= ((b << 4) & 0xf0) << 0;
 IO0SET |= bit(2); //rs=1
 IO0CLR |= bit(1); //rw=0
 IO0SET |= bit(3); //en=1
 lcd_delay();
 IO0CLR |= bit(3); //en=0
}
void show(unsigned char *s)
{
 while(*s) {
 dat(*s++);
 }
}
void lcd_delay()
 {
 unsigned int i;
 for(i=0;i<=1000;i++);
}
/* ---------- DC Motor ---------*/
void forward()
{
 
 IO0SET = bit(16) | bit(18);
 IO0CLR = bit(17); 
}
void reverse()
{
 
 IO0SET = bit(17) | bit(18);
 IO0CLR = bit(16); 
}
void stop()
{
 IO0CLR = bit(18); 
}
