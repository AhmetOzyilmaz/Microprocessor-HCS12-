/*  

   1)  PORTB LEDS buzzerdan muzik �terken Kullan�lacak
   2)  7 SEGMENT CONNECTED PORTB  PLAYER - COMPUTER Skorlar� g�sterilecek s�ras�yla
   3)   DIP SWITCHES PTH BAGLANICAK SEC�M �SLEMLER�  �C� KULLANILACAK 
   4)   Kullan�lacak olan delayler timer module timer overflow kullan�lacak
   5)   timer module output compare and/or input capture 
   6)  �ntterrupts kullan�lacak 
   7)  Serial communication tera term �zerinden de oynanabilmesi i�in kurulcak
   8)  //muhtemelen sens�r kullanm�cam
   9)   LCD Kullan�lacak bilgilendirme yaz�lar� ��kacak
   10)  Player 1 kazan�rsa ba�ka bir muzik computer say� al�rsa ba�ka bir m�zik �alacak
    
    
    Oyuna Baslamak icin dip switch 0dan 1 getiricez en son bitini 
    Sonra oyun ba�layacak dip switchde kullanmis olduk.
    Daha sonra oyunda random kelimeler kolaydan zora gelicek
    6 yanlis ust uste yapilirsa oyun biticek!!!
    Kaynak olarak alinan siteler
    http://www.microdigitaled.com/HCS12/Hardware/Dragon12/CodeWarrior/CW_Keypad_to_PORTB_CProg.txt
    https://bitbucket.org/makiftasova/cse336-microprocessor-laboratory/src/8a5ea7fed1072ec19779809ec4a9009fbd327f98/TermProject/Sources/main.c?at=master


*/


#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */


#define D50US 133 /* Inner loop takes 9 cycles; */


 /// hw 2 part
void delay_50us(int n); /* Delay n 50 microsecond intervals */
void delay_1ms(int n); /* Delay n 1 millisecond intervals */
void Melo(void);
void Melo1(void);
void Melo2(void);
void  show7Seg(unsigned int recived,unsigned int ledNum);

 void delayTimerDelay5s(); // 5snye delay 
void delayTimerDelay10s();//delay for 10 second
void delayTimerDelay();     
void TimerDelay();
                        
/*  Do	261,6
Re	293,7
Mi	329,6
Fa	349,2
Sol	392,0
La	440,0
Si	493,9
Do	523,5      */              


//Buzzer ritimleri
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

 //buzer ritimleri
//ac kapa interrrrrrupt

#define disable() __asm(sei)
#define anable() __asm(cli)



//////////////////////






#define LCD_DAT PORTK /*Port K drives LCD data pins, E, and RS */
#define LCD_DIR DDRK /*Direction of LCD port */
#define LCD_E 0x02 /*LCD E signal */
#define LCD_RS 0x01 /*LCD Register Select signal */
#define CMD 0 /*Command type for put2lcd */
#define DATA 1 /*Data type for put2lcd */
/* Prototypes for functions in lcd.c */
void openlcd(void); /* Initialize LCD display */
void reset(void);
void put2lcd(char c, char type); /*Write command /data to LCD controller */
void puts2lcd (char *ptr); /* Write a string to the LCD display */
void delay_50us(int n); /* Delay n 50 microsecond intervals */
void delay_1ms(int n); /* Delay n 1 millisecond intervals */
void yerlestir(char arr[]);
void manupileEt(char harf);
void SerTx1(unsigned char c);
void randomKelimeUret();
void terminaleBas(char c[],int size);
void kp_init(void);
int kp_get_key(void);
void buzer(int a);
void startM(void);
void bitir(void);
int randomNum(void);
void sayiBas(int skor);
//15 tane array var
//14 tane kelimenden olusuyorlar random secicem bunlari
char array2[14][2] = {{"am"},{"we"} ,{"he"}, {"go"},{"me"}, {"on"}, {"up"}, 
                      {"um"}, {"an"}, {"jo"}, {"ka"}, {"ki"}, {"ox"}, {"ti"}  };
                      
char array3[14][3] = {{"are"},{"lol"} ,{"two"}, {"map"} ,{"tag"} ,{"poi"}, {"sup"} ,
                      {"zoo"},{"moc"} ,{"mod"}, {"ria"} ,{"job"} ,{"joe"}, {"ops"}  };

char array4[14][4] = {{"fuss"},{"gain"},{"gale"},{"juts"},{"keep"},{"rove"} ,{"zoos"} ,  
                      {"glar"},{"able"},{"flax"},{"flex"},{"keen"},{"loud"} ,{"zoom"} };

char array5[14][5] = {{"yards"},{"swarm"},{"waked"},{"mares"},{"skiff"},{"canon"},{"coves"} ,
                      {"wagon"},{"wedge"},{"smash"},{"pound"},{"salts"},{"mused"},{"cells"} };

char array6[14][6] = {{"amazer"},{"cudgel"},{"cuffed"},{"glover"},{"logged"},{"piracy"},{"slopes"},
                        {"zombie"},{"zodiac"},{"winded"},{"yeoman"},{"zircon"},{"zebras"},{"wobble"} };

char array7[14][7] = {{"abdomen"},{"abiding"},{"ability"},{"brother"},{"nuggets"},{"lawyers"},{"hammers"}  ,   
                       {"slapped"},{"restart"},{"restage"},{"resolve"},{"rebuilt"},{"chopped"},{"explode"}   };

char array8[14][8] = {{"authored"},{"apothegm"},{"audacity"},{"bareness"},{"bassinet"},{"castrate"},{"connoted"},
                       {"deceives"},{"depraved"},{"desserts"},{"flinched"},{"indirect"},{"outstrip"},{"penalize"},
                           };

char array9[14][9] = {{"asymmetry"},{"barbecues"},{"bryophyte"},{"conserves"},{"expecting"},{"infecting"},{"ludicrous"} ,
                      {"midpoints"},{"oligocene"},{"poppycock"},{"regiments"},{"seriously"},{"subscript"},{"unspoiled"}   };

char array10[14][10] = {{"aberration"},{"antinomian"},{"backstairs"},{"backstroke"},{"bequeathal"},{"boatswains"},{"clinically"} ,
                          {"congenital"},{"earthquake"},{"fragrantly"},{"intimidate"},{"patriarchs"},{"speechless"},{"undertakes"}  };

char array11[14][11] = {{"anencephaly"},{"accumulated"},{"aimlessness"},{"antagonisms"},{"bottlenecks"},{"burglarizes"},{"conformists"} ,
                          {"descendents"},{"duplicative"},{"fiddlestick"},{"headhunters"},{"innoculates"},{"metalworker"},{"paternoster"}  };

char array12[14][12] = {{"absoluteness"},{"abstractions"},{"antioxidants"},{"bushwhacking"},{"compensating"},{"counterpoint"},{"demonstrator"} ,
                          {"digitization"},{"dishonorable"},{"electrolytes"},{"exacerbating"},{"pressurizing"},{"unreasonable"},{"withholdings"}  };

char array13[14][13] = {{"alphabetizing"},{"alternatively"},{"applicability"},{"changeability"},{"canonicalizes"},{"decentralized"},{"developmental"} ,
                          {"electrocuting"},{"electrocution"},{"electrophorus"},{"exterminating"},{"individualism"},{"microorganism"},{"noncumulative"}  };
                          
                          
char array14[14][14] = {{"appropriations"},{"approximations"},{"archaeological"},{"claustrophobia"},{"clearinghouses"},{"climatologists"},{"credentialisms"} ,
                        {"denominational"},{"disappointment"},{"indigenousness"},{"individualists"},{"methodological"},{"worthwhileness"} };

char array15[14][15] = {{"counterexamples"},{"countermeasures"},{"conscientiously"},{"electrification"},
                        {"electrochemical"},{"fundamentalists"},{"identifications"}  ,{"misapprehension"} ,
                          {"misappropriated"},{"preservationist"},{"psychochemicals"},
                          {"responsibleness"},{"unconsciousness"},{"weatherproofing"}};

char array16[14][16] = {{"antagonistically"},{"canonicalization"},{"distributorships"},{"multiprogramming"},
                        {"unidirectionally"},{"unrepresentative"},{"nonbiodegradable"}  ,{"sensationalizing"} ,
                        {"antagonistically"},{"canonicalization"},{"distributorships"},{"multiprogramming"},
                        {"unidirectionally"},{"unrepresentative"} };
                        

int keypadNum = 0,keypadNum2 = 0,keypadNum3 = 0,keypads = 0;
char kelime[16] = "lanlunAn";       //_a____a_
char dusdu[16] = "Level Dusdun";       //_a____a_      
char hosgeldin[15] = "--WELCOME--";
char dip[20] = "Dip switch 1 yap";
char aferin[15] = "Aferin Sana ;)";     //_a____a_
int sizeKelime = 2;
char showWord[16] = "______________";   //_a____a_
int puan = 2000;




int melody[] = { dos,re,mi,fa,sol,la,do2,do3,do2,
				 do2,do2,sus,do2,do2,la,do2,do3,sus,do2,
				 sus,la,sus,sus,sol,do2,do2,fa,do2,do2,
				 dos,sus,re,sus,dos,dos,do2,sol,do2,do3,
				 dos,sus,do3,do3,sus,do3,sus,la,do2,re,do2,
				 la,do2,do2,sol,do2,do2,do2,do2,do2,dos,
				 re,do2,dos,dos,do2,sol,do2,do3,dos,do2,
				 do3,do3,do2,do2,do2,la,do2,re,sus,do2
				};	//80			
 





#define ROW_NUM 4
#define COL_NUM 4


int lastM = 0;
int PTHs = 0;                  //buzer otturme
int yanlisSayi = 0;           //yanlis sayisi
int randomIndex = 0;          //random kelimemin indexi
int kolaylikIndexi = 2;      // 2 den basla 16 ya kadar ilerleyecek seviye
int flag = 0;
interrupt(((0x10000 - Vtimch5)/2)-1) void timCh5(void);
void winMusic(void);
                 /* 48  0    57  9     asci ch - num*/
void main(void) {
    
    int temp;
     
    char lets = '\0';
    
    DDRB = 0xFF;
    DDRH = 0x00;
    disable();
    TSCR1 = 0x80;     //timer
    
     
    openlcd();
    kp_init();       //keypadi initialize et
    
    puts2lcd(dip);  // oyun baslat 
    //2. Dip switcher are used 
    
    while(PTH != 1);
                       //put2lcd(0xc0 , cmd);//buyuk olcak
    randomIndex = randomNum();//random kelimem
    PORTB = randomIndex;     //kacinci 

    SCI0BDL = 156; /*Set BAUD rate to 9.600*/ 
    SCI0BDH = 0;
    SCI0CR1 = 0x00;      //4c
    SCI0CR2 = 0x0C;   //ox04 du       2c  +
   
    openlcd();
    
    puts2lcd("AHMET");
    put2lcd(0xC0,CMD);
    puts2lcd("OZYILMAZ");
    delay_1ms(150);
    reset();
    
     puts2lcd("111044014");
     put2lcd(0xC0,CMD);
     delay_1ms(150);

    reset();
    puts2lcd("CSE_334");
    put2lcd(0xC0,CMD);
    puts2lcd("Proje");
    delay_1ms(150);
    reset();
    puts2lcd(hosgeldin);
    delay_1ms(150);
    openlcd();
    
    // output For Buzer 
    TFLG1 = 0x20;   //channel 5  clear ??
    TIOS = TIOS | 0x20;
    TCTL1 = TCTL1 & ~0x08;      //ptt5
    TCTL1 = TCTL1 | 0x04;     //tctl = 0000- 01 -00
    TIE = TIE | 0x20;
    ///////////////////////////////
    
     anable();
    startM();
    reset();
    
    
    
  __asm(swi);

} 

/* Terminale string basma !!!!*/      
 void terminaleBas(char c[],int sizeC){
     int i=0;
     
     for(i = 0; i < sizeC; ++i)
          SerTx1(c[i]);
      
    
    SerTx1(' ');
    SerTx1(':');
    SerTx1(' '); 
 
    
    
  }
 void SerTx1(unsigned char c)
  {
    while(!(SCI0SR1 & 0x80));
    SCI0DRL=c;
  }


//string olarak ekranabas
void sayiBas(int skor){
  
  char  arrayim[16] = {0};
  int i = 0;
  int a = 0 , b = 0, c = 0, d = 0; 
  a = skor%10;
  
  if(skor  > 10)
    b = (skor/10)%10;
  if(skor > 100)
    c = (skor/100)%10;
  if(skor > 1000)
    d = (skor/1000)%10;
  
  if(skor  > 1000)
    put2lcd(d + 48,DATA );
  if(skor  > 100)
   put2lcd(c + 48,DATA );
   if(skor  > 10)
    put2lcd(b + 48,DATA );
  
  
  put2lcd(a + 48 ,DATA );
  
   
  
  
}


//oyunu bitirme olayim
void bitir(void){
  
  openlcd();
  puts2lcd("Bitti");
  terminaleBas("Lose",4);
 // winMusic();
  delay_1ms(800); 
  puts2lcd(kelime);
   delay_1ms(200);
   __asm(swi);
  
  
}



// butun harfleri tempru girdi mi

 int temprumu(void){
       int i;
       
       for( i = 0 ; i < sizeKelime; ++i )
               if(showWord[i] !=  kelime[i])
                  return -1;
               
               
               
       return 0;
  
 }


//harf varsa gosterecegim kelimeyi ona gore degistircem
void manupileEt(char harf){
   int i;
   flag = 0;
   for(i = 0 ; i < kolaylikIndexi; ++i){
      if(kelime[i] == harf){
           PTHs = 1;
           flag = 1;
          showWord[i] = harf;
          
      }
     ;
   }
   if(flag == 0)
      PTHs = 3;
     ;
   showWord[kolaylikIndexi] = '\0';
  
}



//ekrani hizli temizlesin diye openlcddeki beklemeleri sildim
void reset(void){
  
  put2lcd(0x28,CMD); /* set 4-bit data, 2-line display, 5x7 font */
  put2lcd(0x0F,CMD); /* turn on display, cursor, blinking */
  put2lcd(0x06,CMD); /* move cursor right */
  put2lcd(0x01,CMD); /* clear screen, move cursor to home */
  delay_1ms(2); /* wait until "clear display" command */ 
  
  
}



void openlcd(void)
{
  LCD_DIR = 0xFF; /* configure LCD_DAT port for output 1. */
  delay_1ms(100); /* Wait for LCD to be ready    1.*/
  put2lcd(0x28,CMD); /* set 4-bit data, 2-line display, 5x7 font */
  put2lcd(0x0F,CMD); /* turn on display, cursor, blinking */
  put2lcd(0x06,CMD); /* move cursor right */
  put2lcd(0x01,CMD); /* clear screen, move cursor to home */
  delay_1ms(2); /* wait until "clear display" command */
  /* complete */
}
void puts2lcd (char *ptr) {
	while (*ptr) { /* While character to send */
		put2lcd(*ptr,DATA); /* Write data to LCD */
		delay_50us(1); /* Wait for data to be written */
		ptr++; /* Go to next character */
	}
}
void put2lcd(char c, char type) {
	char c_lo, c_hi;
	c_hi = (c & 0xF0) >> 2; /* Upper 4 bits of c */
	c_lo = (c & 0x0F) << 2; /* Lower 4 bits of c */
	
	if (type == DATA) LCD_DAT |= LCD_RS; /* select LCD data register */
	else LCD_DAT &= (~LCD_RS); /* select LCD command register */
	
	if (type == DATA)
		LCD_DAT = c_hi|LCD_E|LCD_RS; /* output upper 4 bits, E, RS high */
	else
		LCD_DAT = c_hi|LCD_E; /* output upper 4 bits, E, RS low */
	LCD_DAT |= LCD_E; /* pull E signal to high */
	__asm(nop); /* Lengthen E */
	__asm(nop);
	__asm(nop);
 
	LCD_DAT &= (~LCD_E); /* pull E to low */
	if (type == DATA)
		LCD_DAT = c_lo|LCD_E|LCD_RS; /* output lower 4 bits, E, RS high */
	else
		LCD_DAT = c_lo|LCD_E; /* output lower 4 bits, E, RS low */
	LCD_DAT |= LCD_E; /* pull E to high */
	__asm(nop); /* Lengthen E */
	__asm(nop);
	__asm(nop);
 
	LCD_DAT &= (~LCD_E); /* pull E to low */
	delay_50us(1); /* Wait for command to execute */
}

/* need 50x24 = 1200 cycles */


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


 // 0 --- 6 arasi degistir
 //random sayilar ureten fonksiyonumuz
int randomNum()
{
  unsigned int k;
  unsigned int c;
  for(k = 0 ; k < 11; ++k);
  
  k = ((rand() + 4 + TCNT) );
  c = k % 15;
  return c;
  
}


 // size gore o sizeda random kelime vericek
void randomKelimeUret(){
  switch( kolaylikIndexi ){
  
  

    
 case 2:
      yerlestir(array2[randomIndex]);
      break;
 case 3:
      yerlestir(array3[randomIndex]);
      break;
 case 4:
      yerlestir(array4[randomIndex]);
      break;
 case 5:
      yerlestir(array5[randomIndex]);
      break;
 case 6:
      yerlestir(array6[randomIndex]);
      break;
 case 7:
      yerlestir(array7[randomIndex]);
      break;
 case 8:
      yerlestir(array8[randomIndex]);
      break;
 case 9:
      yerlestir(array9[randomIndex]);
      break;
 case 10:
      yerlestir(array10[randomIndex]);
      break;
 case 11:
      yerlestir(array11[randomIndex]);
      break;
 case 12:
      yerlestir(array12[randomIndex]);
      break;
 case 13:
      yerlestir(array13[randomIndex]);
      break;
 case 14:
      yerlestir(array14[randomIndex]);
      break;     
 case 15:
      yerlestir(array15[randomIndex]);
    break;
 case 16:
      yerlestir(array16[randomIndex]);
 case 17:
      winMusic();
    break;
    

  }
}
 
 //random kelimeyi yerlestiriyorum
void yerlestir(char arr[]){



    int i;
    
    
    for(i = 0; i < kolaylikIndexi; ++i) {
           showWord[i] = '_';
           kelime[i] =  arr[i];
    }
    kelime[i] = '\0' ;
    showWord[i] = '\0';
    

  
}
 //mario gibi calicak tam degil ama 
void winMusic(void){
	int i;
	int secD = 83,secD2 = 11;

	terminaleBas(" Win Game  " , 9);
	openlcd();
	puts2lcd("** Kazandin **");
	PTHs = 0;
	delay_1ms(secD2);
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


/**
* Mehmet Akif tasove github sayfasindan destek alinmistir keypad icin!!!
*/
void kp_init(void)
{
   DDRA = 0x0F; // enable keypad 
}

int kp_get_key(void) 
{
  int row, column;
               
  const char row_mask[ROW_NUM] = { 0xFE,0xFD,0xFB,0xF7 };
  const char col_mask[COL_NUM] = { 0x10,0x20,0x40,0x80 };        
  const unsigned int keys[ROW_NUM][COL_NUM] = { 1,2,3,10,
                                                4,5,6,11,
                                                7,8,9,12,
                                                14,0,15,13 };
    
  for(;;){ // wait until input
      
    for(row=0 ; row < ROW_NUM ; ++row){
         
      PORTA = 0xFF;// Reset PortA
      for(column=0; column < COL_NUM ;++column){
                
        PORTA = PORTA & row_mask[row]; // Set specific row to 0
                                     
        if( (PORTA & col_mask[column]) == 0 ){
        // Check if any key is pressed
           
          delay_1ms(1);                      
          // Wait 1ms and check again for make sure key is pressed.
          if( (PORTA & col_mask[column]) == 0 ){
            return keys[row][column]; 
          }                   
              
        }
           
      }
      
    }
  } 
}