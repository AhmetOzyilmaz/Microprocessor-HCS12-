/*  

   1)  PORTB LEDS buzzerdan muzik öterken Kullanýlacak
   2)  7 SEGMENT CONNECTED PORTB  PLAYER - COMPUTER Skorlarý gösterilecek sýrasýyla
   3)   DIP SWITCHES PTH BAGLANICAK SECÝM ÝSLEMLERÝ  ÝCÝ KULLANILACAK 
   4)   Kullanýlacak olan delayler timer module timer overflow kullanýlacak
   5)   timer module output compare and/or input capture 
   6)  ýntterrupts kullanýlacak 
   7)  Serial communication tera term üzerinden de oynanabilmesi için kurulcak
   8)  //muhtemelen sensör kullanmýcam
   9)   LCD Kullanýlacak bilgilendirme yazýlarý çýkacak
   10)  Player 1 kazanýrsa baþka bir muzik computer sayý alýrsa baþka bir müzik çalacak
    
    
    Oyuna Baslamak icin dip switch 0dan 1 getiricez en son bitini 
    Sonra oyun baþlayacak dip switchde kullanmis olduk.
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

int melody[] = { la,la,sol,fa,fa,sol,la,sol,sol,fa,mi,mi,fa,sol,fa,fa,mi,re,re,mi,fa,fa,fa,mi,fa,re,re,re,re,re,re};	//80		 

int melody1[] = { la,la,sol,fa,fa,sol,la,sol,sol,fa,mi,mi,fa,sol,fa,fa,mi,re,re,mi,fa,fa,fa,mi,fa,re,re,re,re,re,re};	//80		 
 
int melody2[] = {sol,sol,sol,sol,sol,sol,sol,fa,mi,fa,mi,re,re,re,fa,mi,fa,mi,re,do2,sol,fa,mi,fa,mi,re,re,re,re};	//80		 

 int melody3[] = {re,mi,fa,sol,fa,sol,sol,sol,fa,sol,sol,sol,fa,mi,re,mi,fa,sol,mi,fa,re,mi,do2,do2,fa,mi,re,re};


int lastM = 0;
int PTHs = 0;                  //Buzzer otturme
int flag = 0;
interrupt(((0x10000 - Vtimch5)/2)-1) void timCh5(void);
void winMusic(void);


////Final Project New Function /////

  void BuzzerSettingAndCallMelo() ;
  void  show7Seg(unsigned int recived,unsigned int ledNum);



int UserScore  = 0;
int ComputerScore = 0;


    //////////////////////
  // LCD Settings 
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
                        

int keypadNum = 0;
char kelime[16] = "lanlunAn";       //_a____a_
char dusdu[16] = "Level Dusdun";       //_a____a_      
char hosgeldin[15] = "--WELCOME--";
char dip[20] = "Dip switch 1 yap";
char aferin[15] = "Aferin Sana ;)";     //_a____a_
int sizeKelime = 2;
char showWord[16] = "______________";   //_a____a_
int puan = 2000;




#define ROW_NUM 4
#define COL_NUM 4



int yanlisSayi = 0;           //yanlis sayisi
int randomIndex = 0;          //random kelimemin indexi
int kolaylikIndexi = 2;      // 2 den basla 16 ya kadar ilerleyecek seviye
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

    //openlcd();
    
     BuzzerSettingAndCallMelo();
     anable();
     openlcd();
    puts2lcd("TAS KAGIT");
    put2lcd(0xC0,CMD);
    puts2lcd("MAKAS OYUNU");
    delay_1ms(150);
     reset();
     
     openlcd();
      
    puts2lcd("TAS ISE 1 E BAS");
    put2lcd(0xC0,CMD);
    delay_1ms(75);
    
    reset(); 
    openlcd();
    
    puts2lcd("KAGIT ISE");
    put2lcd(0xC0,CMD);
    puts2lcd("2 YE BAS");
        delay_1ms(75);

    reset(); 
    openlcd();
     
    puts2lcd("MAKAS ISE");
    put2lcd(0xC0,CMD);
    puts2lcd("3 E BAS");
    delay_1ms(75);
    reset();   
    

        while(1){
                 
                  openlcd();
                  puts2lcd("TAS - KAGIT - MAKAS ");
                  put2lcd(0xC0,CMD);
                  puts2lcd("HANGISI");
                  delay_1ms(75);
                  reset();   
                 PORTB = randomIndex;     //kacinci olduðu port b  gösteriliyor 
                 keypadNum = kp_get_key(); // kullanýcýdan bir seçim alýyor . 
                 randomIndex = randomNum();//random sayý taþ kaðýt makas için
                 
                    if(randomIndex == 1 && keypadNum == 1 ){ // Bilgisayar Taþ 
                          //TEKRAR KAZANAN YOK
                             delay_1ms(75);
 
                    } else if(randomIndex == 1 && keypadNum == 2 ){ // Bilgisayar Taþ 
                          //USER WÝN
                         ++ UserScore; 
                    } else if(randomIndex == 1 && keypadNum == 3 ){ // Bilgisayar Taþ 
                          //COMPUTER WÝN
                          ++ComputerScore ;
                    } else  if(randomIndex == 2  && keypadNum == 1){ // Bilgisayar KAGIT 
                          // COMPUTER WIN
                         ++ ComputerScore  ;
                    } else  if(randomIndex == 2  && keypadNum == 2){ // Bilgisayar KAGIT 
                             //TEKRAR KAZANAN YOK
                    }else  if(randomIndex == 2 && keypadNum == 3 ){ // Bilgisayar KAGIT 
                             //  USER WIN
                              ++ UserScore; 
                    }
                     else if(randomIndex == 3  && keypadNum == 1){   // Bilgisayar MAKAS
                           //USER WÝN
                           ++ UserScore; 

                    } else if(randomIndex == 3  && keypadNum == 2){   // Bilgisayar MAKAS
                           // COMPUTER WIN
                         ++ComputerScore;
                    } else if(randomIndex == 3  && keypadNum == 3){   // Bilgisayar MAKAS
                          //TEKRAR KAZANAN YOK

                    }
                    
                      openlcd();
                      puts2lcd("USER SCORE ");
                      put2lcd(0xC0,CMD);
                      puts2lcd("LOOK 7 SEGMENT ");

                      delay_1ms(75);
                      reset();    
                      show7Seg(UserScore,215);   // en sol 7 segment user score u
                      delayTimerDelay();
                      openlcd();
                      puts2lcd("COMPUTER SCORE ");
                      put2lcd(0xC0,CMD);
                      puts2lcd("LOOK 7 SEGMENT ");
                      delay_1ms(75);
                      reset();  
                      show7Seg(ComputerScore,254);   //en sað 7 segment computer scoru
                      delayTimerDelay() ;
                    
        }

    
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
  c = k % 3;
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

   /////////////////////final Project

  void BuzzerSettingAndCallMelo() {
                // output For Buzer 
    TFLG1 = 0x20;   //channel 5  clear ??
    TIOS = TIOS | 0x20;
    TCTL1 = TCTL1 & ~0x08;      //ptt5
    TCTL1 = TCTL1 | 0x04;     //tctl = 0000- 01 -00
    TIE = TIE | 0x20;
    ///////////////////////////////
    
      anable();
      Melo2();   
      disable();
 }
 
 
 
 



     /////////////////////final Project

   /////////////////////Hw2
 //Timer Delay 
void delayTimerDelay5s(){ // 5snye delay 
  int i = 0;
  for(;i<5;++i )   //
    delayTimerDelay();
}

 //Timer Delay 
void delayTimerDelay10s(){ // 5snye delay 
  int i = 0;
  for(;i<2;++i )   //10 sn ye delay 
    delayTimerDelay5s();
}


//Timer Delay 
void delayTimerDelay(){
  int i = 0;
  for(;i<367;++i )   //24 000 000 / 656 1 saniye  ye
    TimerDelay();
}
//Timer Overflow Function
void TimerDelay(){
       TFLG2 = 0X80;
     while(!(TFLG2 & 0X80));{
     }
     TFLG2 = 0X80 | TFLG2;

}



    void  show7Seg(unsigned int recived,unsigned int ledNum){
   DDRP = ledNum;
   DDRB = 0XFF; //PORTB OUTPUT
   
     
   switch(recived){
    case 0:
    PORTB = 0b00111111;
    break;
    
    case 1:
    PORTB = 0b00000110;
    break;
    case 2:
    PORTB = 0b01011011;    
    break;
   
    case 3:
    PORTB = 0b10001111;    

    break;
    
    case 4:
    PORTB = 0b01100110;       
    break;
    
    case 5:
    PORTB = 0b01101101;       
    break;   

    case 6:
    PORTB = 0b01111101;       

    break;
    
    case 7:
 
    PORTB = 0b00000111;       
  
    break;
    
    case 8:
    PORTB = 0b01111111;

    break;
    
    case 9:
    PORTB = 0b01101111;

    break;
   }
   
}

void delay_50us(int n)
{
  volatile int i;
  for (;n>0;n--)
    for (i=0 ;i<133;++i) ;
}



void delay_1ms(int n)
{
  for (;n>0;n--) delay_50us(200);
}

//intterrupt kullanýldý.     
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





void Melo(void){
  int secD = 83,i,j;
	PTHs = 0;
	PTHs = 4; //lastM calicak
	for(j = 0; j <1; ++j){
  	for(i = 0; i < 28; ++i){
  		lastM = melody3[i];
  		  PORTB = lastM;

  		delay_1ms(1000/secD);
  	}
  }
	PTHs = 0;
} 
void Melo1(void){
  int secD = 83,i,j;
	PTHs = 0;
	PTHs = 4; //lastM calicak
	for(j = 0; j < 1; ++j){
  	for(i = 0; i < 28; ++i){
  		lastM = melody1[i];
  		PORTB = lastM;
  		delay_1ms(1000/secD);
  	}
  }
	PTHs = 0;
}
void Melo2(void){
  int secD = 83,i,j;
	PTHs = 0;
	PTHs = 4; //lastM calicak
	for(j = 0; j <1 ; ++j){
  	for(i = 0; i < 28; ++i){
  		lastM = melody2[i];
  	  PORTB = lastM;

  		delay_1ms(1000/secD);
  	}
  }
	PTHs = 0;
  
  
}
