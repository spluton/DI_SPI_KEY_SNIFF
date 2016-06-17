#include <SPI.h>

uint8_t buf [32];
volatile byte pos;
volatile bool process_it;

void PrintHex(const uint8_t *data, const uint32_t numBytes)
{
    for (uint8_t i = 0; i < numBytes; i++) {
        if ((data[i]&0xFF) < 0x10) {
            Serial.print(" 0");
        } else {
            Serial.print(' ');
        }
        Serial.print(data[i], HEX);
    }
    Serial.println("");
}


void setup (void)
{
  Serial.begin (115200);   // debugging
  
 
    while (!Serial);
  // turn on SPI in slave mode
  SPCR |= bit (SPE);
 Serial.println("Keys");
  // have to send on master in, *slave out*


  // get ready for an interrupt
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
byte c = SPDR;  // grab byte from SPI Data Register
  if (pos < sizeof buf){  // add to buffer if room
    if (buf[0]==0x0A){
    buf [pos] = c;
    pos++;
    }

    if (pos==0){
      buf [0] = c;
      pos++;
    }
    // example: newline means time to process buffer
    }
   else
   pos=0;// end of room available
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (digitalRead(10)){
    //if ((buf[0]!=0x09)|(buf[1]!=0x00)|(buf[14]!=0x00)|((buf[15]!=0x02)&(pos>=15)))
    if ((buf[0]!=0x0A)|(buf[12]!=0x00)|((buf[13]!=0x02)&(pos>=13))){
    pos=0;
    buf[0]=0x00;
    buf[12]=0x00;
    buf[13]=0x00;
    }
    if (pos>=13){
    if (buf[1] < 0x10)
    Serial.print("0");
    Serial.print(buf[1],HEX);
    if (buf[3] < 0x10)
    Serial.print("0");
    Serial.print(buf[3],HEX);
    if (buf[5] < 0x10)
    Serial.print("0");
    Serial.print(buf[5],HEX);
    if (buf[7] < 0x10)
    Serial.print("0");
    Serial.print(buf[7],HEX);
    if (buf[9] < 0x10)
    Serial.print("0");
    Serial.print(buf[9],HEX);
    if (buf[11] < 0x10)
    Serial.print("0");
    Serial.println(buf[11],HEX);
    
    //PrintHex(buf,14);
    pos = 0;
    }  // end of flag set

  }}  // end of loop
