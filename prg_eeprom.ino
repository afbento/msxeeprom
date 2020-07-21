#define D0 2
#define D1 3
#define D2 4
#define D3 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9  
#define LE1 12 // habilita gravacao no primeiro 74ls373 em HIGH
#define LE2 11 // habilita gravacao no segundo 74ls373 em HIGH
#define CE3 10 //habilita 74ls245 em LOW
#define WE 13 // habilita ecrita no 28c256 em LOW


void Escrita_EEPROM(unsigned int endr,byte dado) {
   int i;
  
   digitalWrite(WE,HIGH);
   digitalWrite(CE3,HIGH);
   
   digitalWrite(LE1,LOW);
   digitalWrite(LE2,HIGH);
   
   byte msb=(endr>>8);
   for(i=D0;i<=D6;i++) {digitalWrite(i,msb&1); msb=msb>>1; }
   digitalWrite(D7,HIGH); // A->B no 74ls245, e desabilita leitura no 28c256

   digitalWrite(LE1,HIGH);
   digitalWrite(LE2,LOW);
   byte lsb=(endr&255);
   for(i=D0;i<=D7;i++) {digitalWrite(i,lsb&1); lsb=lsb>>1; }
   

   digitalWrite(LE1,LOW);
   digitalWrite(LE2,LOW);

   digitalWrite(CE3,LOW);
   for(i=D0;i<=D7;i++) {digitalWrite(i,dado&1); dado=dado>>1; }
   

   digitalWrite(WE,LOW);
   delayMicroseconds(1);
   //delay(10);
   digitalWrite(WE,HIGH);
   digitalWrite(CE3,HIGH);
   digitalWrite(D7,LOW); // B->A no 74ls245, e habilita leitura no 28c256
   delay(10);   
  
}

byte Leitura_EEPROM(unsigned int endr) { 
   int i;
   byte dado;

   
   digitalWrite(WE,HIGH);
   digitalWrite(CE3,HIGH);
   
   digitalWrite(LE1,LOW);
   digitalWrite(LE2,HIGH);
   
   byte msb=(endr>>8);
   for(i=D0;i<=D6;i++) {digitalWrite(i,msb&1); msb=msb>>1; }
   digitalWrite(D7,LOW); // B->A no 74ls245, e habilita leitura no 28c256

   digitalWrite(LE1,HIGH);
   digitalWrite(LE2,LOW);
   byte lsb=(endr&255);
   for(i=D0;i<=D7;i++) {digitalWrite(i,lsb&1); lsb=lsb>>1; }
   
   
   
   digitalWrite(LE1,LOW);
   digitalWrite(LE2,LOW);

   digitalWrite(CE3,LOW);

   for(i=D0;i<=D7;i++) pinMode(i,INPUT);

   
   dado=digitalRead(D7)*128+digitalRead(D6)*64+digitalRead(D5)*32+digitalRead(D4)*16+digitalRead(D3)*8+digitalRead(D2)*4+digitalRead(D1)*2+digitalRead(D0)*1;

   
   for(i=D0;i<=D7;i++) pinMode(i,OUTPUT);
 
   digitalWrite(CE3,HIGH);
   
   return dado;
}

void setup() {
  // put your setup code here, to run once:
  int e,i;
  
  pinMode(LE1,OUTPUT);
  pinMode(LE2,OUTPUT);
  pinMode(CE3,OUTPUT);
  pinMode(WE,OUTPUT);
  digitalWrite(WE,HIGH);
  

  for(i=D0;i<=D7;i++) pinMode(i,OUTPUT);

  Serial.begin(9600);

  
  

  /*

  Serial.println("teste");
  
  //for(e=0;e<32768;e++) {Escrita_EEPROM(e,e%15);Serial.println("escrito End.");Serial.println(e);}
 
  Serial.println("acabou!!");
*/
}

void loop() {
  unsigned int e,op;
  byte d,f,c,c1;
/*
  for(e=0;e<32768;e++) {Escrita_EEPROM(e,e);Serial.println(Leitura_EEPROM(e),HEX);}
  Serial.println(" escrito");
  
  for(e=0;e<32768;e++) {
  d=Leitura_EEPROM(e);
  Serial.print(d,HEX);Serial.print(" ");
  }
  Serial.println("lido ");*/
  
 while (Serial.available() > 0) {
      //e=Serial.parseInt();
      c1=Serial.readBytes(&c,1);
      if(c=='G') {
        e=Serial.parseInt();
        d=Serial.parseInt();
        //Serial.print("gravando... End.=");Serial.print(e,HEX);Serial.print(" dado=");Serial.println(d,HEX);
        Escrita_EEPROM(e,d);
        delay(1);
        f=Leitura_EEPROM(e);
        if(f==d) Serial.println("G"); else Serial.println("E"); 
        
        }

      if(c=='L') {
        e=Serial.parseInt();
        d=Leitura_EEPROM(e);
        Serial.println(d);
        }     
      
 }
} 
