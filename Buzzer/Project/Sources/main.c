#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#define D50US 133 /* Inner loop takes 9 cycles; */

void delay_50us(int n); /* Delay n 50 microsecond intervals */
void delay_1ms(int n); /* Delay n 1 millisecond intervals */
void buzer(int a);
void startM(void);
                        
/*  Do	261,6
Re	293,7
Mi	329,6
Fa	349,2
Sol	392,0
La	440,0
Si	493,9
Do	523,5                    
 binary       dec                
110 0001	141	97	61	a
110 0010	142	98	62	b
110 0011	143	99	63	c
110 0100	144	100	64	d
110 0101	145	101	65	e
110 0110	146	102	66	f
110 0111	147	103	67	g
110 1000	150	104	68	h
110 1001	151	105	69	i
110 1010	152	106	6A	j
110 1011	153	107	6B	k
110 1100	154	108	6C	l
110 1101	155	109	6D	m
110 1110	156	110	6E	n
110 1111	157	111	6F	o
111 0000	160	112	70	p
111 0001	161	113	71	q
111 0010	162	114	72	r
111 0011	163	115	73	s
111 0100	164	116	74	t
111 0101	165	117	75	u
111 0110	166	118	76	v
111 0111	167	119	77	w
111 1000	170	120	78	x
111 1001	171	121	79	y
111 1010	172	122	7A	z
 */
int keypadNum = 0,keypadNum2 = 0,keypadNum3 = 0,keypads = 0;
int sizeKelime = 2;
char showWord[16] = "______________";   //_a____a_

//buzer ritimleri
#define ilk 40000
#define orta 25000
#define son 10000

#define dos	45977
#define re	45570
#define mi	36363
#define fa	34280
#define sol	30612
#define la	27272
#define do2	24490
#define do3	20490
#define sus 10000

#define noteDuration1 12
#define noteDuration2 9
int melody[] = { dos,re,mi,fa,sol,la,do2,do3,do2,
				 do2,do2,sus,do2,do2,la,do2,do3,sus,do2,
				 sus,la,sus,sus,sol,do2,do2,fa,do2,do2,
				 dos,sus,re,sus,dos,dos,do2,sol,do2,do3,
				 dos,sus,do3,do3,sus,do3,sus,la,do2,re,do2,
				 la,do2,do2,sol,do2,do2,do2,do2,do2,dos,
				 re,do2,dos,dos,do2,sol,do2,do3,dos,do2,
				 do3,do3,do2,do2,do2,la,do2,re,sus,do2
				};	//80			
 

/*Half note               =  120 / BPM
Quarter note            =   60 / BPM
Eighth note             =   30 / BPM
Sixteenth note          =   15 / BPM
Dotted-quarter note     =   90 / BPM
Dotted-eighth note      =   45 / BPM
Dotted-sixteenth note   = 22.5 / BPM
Triplet-quarter note    =   40 / BPM
Triplet-eighth note     =   20 / BPM
Triplet-sixteenth note  =   10 / BPM*/

//buzer ritimleri
//ac kapa interrrrrrupt
#define disable() __asm(sei)
#define anable() __asm(cli)


int lastM = 0;
int PTHs = 0;                  //buzer otturme
int flag = 0;
interrupt(((0x10000 - Vtimch5)/2)-1) void timCh5(void);
void winMusic(void);

void main(void) {

            
    DDRB = 0xFF;
    DDRH = 0x00;
    disable();
    TSCR1 = 0x80;     //timer
  
    
    
    SCI0BDL = 26; /*Set BAUD rate to 9.600*/ 
    SCI0BDH = 0;
    SCI0CR1 = 0x00;      //4c
    SCI0CR2 = 0x0C;   //ox04 du       2c  +

    
    // output For Buzer 
    TFLG1 = 0x20;   //channel 5  clear ??
    TIOS = TIOS | 0x20;
    TCTL1 = TCTL1 & ~0x08;      //ptt5
    TCTL1 = TCTL1 | 0x04;     //tctl = 0000- 01 -00
    TIE = TIE | 0x20;
    ///////////////////////////////
    
    
    anable();
    winMusic();
    startM();
    
  __asm(swi);


} 

void delay_50us(int n)
{
  volatile int c;
  for (;n>0;n--)
  for (c=D50US;c>0;c--) ;
}



void delay_1ms(int n)
{
  for (;n>0;n--) delay_50us(200);
}

     
interrupt(((0x10000 - Vtimch5)/2)-1) void timCh5(void){


   if(PTHs == 3) {
      TCTL1 = TCTL1 & ~0x08;      //ptt5
      TCTL1 = TCTL1 | 0x04;     //tctl = 0000- 01 -00
      TC5 = TC5 + ilk;
  
   }
    
    
   if(PTHs == 2)
    {
      TCTL1 = TCTL1 & ~0x08;      //ptt5
      TCTL1 = TCTL1 | 0x04;     //tctl = 0000- 01 -00
      TC5 = TC5 + orta;
  
   }
   if(PTHs == 1)
  {
      TCTL1 = TCTL1 & ~0x08;      //ptt5
      TCTL1 = TCTL1 | 0x04;     //tctl = 0000- 01 -00
      TC5 = TC5 + son;
  
   }
  if(PTHs == 4)
  {
      TCTL1 = TCTL1 & ~0x08;      //ptt5
      TCTL1 = TCTL1 | 0x04;     //tctl = 0000- 01 -00
      TC5 = TC5 + lastM;
  
   }
   if(PTHs == 0)
      TCTL1 = 0x00;

      TFLG1 = 0x20;   //0x02  clear ??

}
void buzer(int time){
  
     int buz;
     for(buz = 0; buz < 11; buz++) 	//100 degistir 		
            {
            
               DDRT = DDRT | 0x70;    // PTT as output  70 i
               PTT = PTT | 0x70;      //make PT0=1
               delay_1ms(time);         //change the delay size to see what happens
               PTT = PTT &(~0x70);      //Make PT0=0
               delay_1ms(time);         //change delay size....

            } 
}

void winMusic(void){
	int i;
	int secD = 83,secD2 = 11;

	PTHs = 4; //lastM calicak
	for(i = 0; i < 80; ++i){
		lastM = melody[i];
		if(i > 40)
			delay_1ms(1000/secD2);
		else
			delay_1ms(1000/secD);
	}
	PTHs = 0;
}
void startM(void){
  int secD = 83,i,j;
	PTHs = 0;
	PTHs = 4; //lastM calicak
	for(j = 0; j < 2; ++j){
  	for(i = 0; i < 32; ++i){
  		lastM = melody[i];
  		delay_1ms(1000/secD);
  	}
  }
	PTHs = 0;
  
  
}
