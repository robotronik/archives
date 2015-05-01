#include <pic18f4550.h>
#include <xc.h>
#include "RFM70_register.h"
#include "init.h"

// RFM70 SPI read and write commands
#define RFM70_CMD_READ_REG            0x00
#define RFM70_CMD_WRITE_REG           0x20

//interrupt status
#define STATUS_RX_DR    0x40
#define STATUS_TX_DS    0x20
#define STATUS_MAX_RT   0x10

#define STATUS_TX_FULL  0x01

//FIFO_STATUS
#define FIFO_STATUS_TX_REUSE  0x40
#define FIFO_STATUS_TX_FULL   0x20
#define FIFO_STATUS_TX_EMPTY  0x10

#define FIFO_STATUS_RX_FULL   0x02
#define FIFO_STATUS_RX_EMPTY  0x01



// Bank0 register initialization values
#define BANK0_ENTRIES 10
const unsigned char Bank0_Reg[ BANK0_ENTRIES ][ 2 ]={
   {  0, 0x0F }, // receive, enabled, CRC 2, enable interupts
   {  1, 0x3F }, // auto-ack on all pipes enabled
   {  2, 0x03 }, // Enable pipes 0 and 1
   {  3, 0x03 }, // 5 bytes addresses
   {  4, 0xff }, // auto retransmission delay 4000 ms, 15 times
   {  5, 0x0A }, // channel 10
   {  6 ,0x37 }, // data rate 1Mbit, power 5dbm, LNA gain high
   {  7, 0x07 }, // why write this at all?? but seems required to work...
   {  8, 0x00 }, // clear Tx packet counters
   { 23, 0x00 }, // fifo status
};

unsigned char rfm70_SPI_RW(unsigned char byte) {
    char value;
    //uint8_t buf;// = SPI1BUF;
    SSPBUF = byte;
    while (!SSPSTATbits.BF);
    value = SSPBUF; //Status from RFM70

    return value;
}


void rfm70_register_write( unsigned char reg, unsigned char value ){
   if( reg < RFM70_CMD_WRITE_REG ){
      reg |= RFM70_CMD_WRITE_REG;
   }
   CSN = 0;               // CSN low, init SPI transaction
   (void)rfm70_SPI_RW( reg );    // select register
   (void)rfm70_SPI_RW( value );  // ..and write value to it..
   CSN = 1;              // CSN high again
}


void rfm70_buffer_write(
   char reg,
   const unsigned char *pBuf,
   unsigned char length
){
   unsigned char i;
   if( reg < RFM70_CMD_WRITE_REG ){
      reg |= RFM70_CMD_WRITE_REG;
   }
   CSN = 0;                     // Set CSN low, init SPI tranaction
   (void)rfm70_SPI_RW( reg );         // Select register to write tio write
   for( i = 0; i < length; i++ ){      // write all bytes in buffer(*pBuf)
      (void)rfm70_SPI_RW( *pBuf++ );  // write one byte
   }
   CSN = 1;                     // Set CSN high again
}

unsigned char rfm70_register_read( unsigned char reg ){
   unsigned char value;
   if( reg < RFM70_CMD_WRITE_REG ){
      reg |= RFM70_CMD_READ_REG;
   }
   CSN = 0;              // CSN low, initialize SPI communication...
   (void)rfm70_SPI_RW( reg ); 
   value = rfm70_SPI_RW( 0 );   // ..then read register value
   CSN = 1; // CSN high, terminate SPI communication
   return value;                 // return register value
}

void rfm70_buffer_read(
   unsigned char reg,
   unsigned char *pBuf,
   unsigned char length
){
   unsigned char i;
   if( reg < RFM70_CMD_WRITE_REG ){
      reg |= RFM70_CMD_READ_REG;
   }
   CSN =  0;                   // Set CSN 0
   (void)rfm70_SPI_RW( reg );       // Select register to write
   for( i = 0; i < length; i++ ){    // read all bytes
      *pBuf++ = rfm70_SPI_RW( 0 );  // read one byte from RFM70
   }
   CSN = 1 ;                   // Set CSN high again
}






void rfm70_bank( unsigned char b ){
   unsigned char st = 0x80 & rfm70_register_read( RFM70_REG_STATUS );
   if(( st &&( b == 0 )) || (( st == 0 ) && b )){
      rfm70_register_write( RFM70_CMD_ACTIVATE, 0x53 );
   }
}

const unsigned char RX0_Address[]={ 0x34, 0x43, 0x10, 0x10, 0x01 };


   // magic bank1 register initialization values
const unsigned long Bank1_Reg0_13[] = {
   0xE2014B40,
   0x00004BC0,
   0x028CFCD0,
   0x41390099,
   0x0B869Ef9,
   0xA67F0624,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00127300,
   0x36B48000 };

   // more magic bank1 register initialization values
const unsigned char Bank1_Reg14[] = {
   0x41, 0x20, 0x08, 0x04, 0x81, 0x20, 0xCF, 0xF7, 0xFE, 0xFF, 0xFF };

   // initialize bank1 like in the example code.
   // don't ask why, just do it
void rfm70_init_bank1( void ){
   unsigned char i, j;
   unsigned char WriteArr[ 12 ];

   rfm70_bank( 1 );

   for( i = 0; i <= 8; i++ ){ //reverse!
      for( j = 0; j < 4; j++ ){
         WriteArr[ j ]=( Bank1_Reg0_13[ i ]>>(8*(j) ) )&0xff;
      }
      rfm70_buffer_write( i,WriteArr, 4 );
   }

   for( i = 9; i <= 13; i++ ){
      for( j = 0; j < 4; j++ ){
         WriteArr[ j ]=( Bank1_Reg0_13[ i ]>>(8*(3-j) ) )&0xff;
      }
      rfm70_buffer_write( i, WriteArr, 4 );
   }

   rfm70_buffer_write( 14, Bank1_Reg14, 11 );

   //toggle REG4<25,26>
   for(j=0;j<4;j++){
      WriteArr[ j ]=( Bank1_Reg0_13[ 4 ]>>(8*(j) ) )&0xff;
   }

   WriteArr[ 0 ] = WriteArr[ 0 ] | 0x06;
   rfm70_buffer_write( 4, WriteArr, 4);

   WriteArr[ 0 ] = WriteArr[ 0 ] & 0xf9;
   rfm70_buffer_write( 4, WriteArr,4);

   rfm70_bank( 0 );
}

unsigned char rfm70_is_present( void ){
   unsigned char st1, st2;
   st1 = rfm70_register_read( RFM70_REG_STATUS );
   rfm70_register_write( RFM70_CMD_ACTIVATE, 0x53 );
   st2 = rfm70_register_read( RFM70_REG_STATUS );
   rfm70_register_write( RFM70_CMD_ACTIVATE, 0x53 );
   return ( st1 ^ st2 ) == 0x80;
}


void rfm70_mode_receive( void ){
   unsigned char value;

   // flush receive queue
   rfm70_register_write( RFM70_CMD_FLUSH_RX, 0 );

   // clear interrupt status
   value = rfm70_register_read( RFM70_REG_STATUS );
   rfm70_register_write( RFM70_REG_STATUS ,value );

   // switch to receive mode
   CE = ( 0 );
   value = rfm70_register_read( RFM70_REG_CONFIG );
   value |= 0x01; // set RX bit
   value |= 0x02; // set PWR_UP bit
   rfm70_register_write( RFM70_REG_CONFIG, value );
   CE = ( 1 );
}

void rfm70_mode_transmit( void ){
   unsigned char value;

   // flush transmit queue
   rfm70_register_write( RFM70_CMD_FLUSH_TX, 0);

   // clear interrupt status
   value = rfm70_register_read( RFM70_REG_STATUS );
   rfm70_register_write( RFM70_REG_STATUS ,value );

   // switch to transmit mode
   CE = ( 0 );
   value = rfm70_register_read( RFM70_REG_CONFIG );
   value &= 0xFE; // clear RX bit
   value |= 0x02; // set PWR_UP bit
   rfm70_register_write( RFM70_REG_CONFIG, value );
   CE = ( 1 );
}

void rfm70_mode_standby( void ){
   unsigned char value;
   CE = ( 0 );
   value = rfm70_register_read( RFM70_REG_CONFIG );
   value |= 0x02; // set PWR_UP bit
   rfm70_register_write( RFM70_REG_CONFIG, value );
}

void rfm70_mode_powerdown( void ){
   unsigned char value;
   CE =( 0 );
   value = rfm70_register_read( RFM70_REG_CONFIG );
   value &= 0xFD; // clear PWR_UP bit
   rfm70_register_write( RFM70_REG_CONFIG, value );
}

void rfm70_channel( unsigned char ch ){
  // MSB must be 0
  rfm70_register_write( RFM70_REG_RF_CH, ch & 0x7E );
}

void rfm70_air_data_rate( unsigned char rate ){
   unsigned char value;
   CE = ( 0 );
   value = rfm70_register_read( RFM70_REG_RF_SETUP );
   value &= 0xF7;
   if( rate > 1 ){
      value |= 0x08;
   }
   rfm70_register_write( RFM70_REG_RF_SETUP, value );
   CE =( 1 );
}

void rfm70_crc_length( unsigned char len ){
   unsigned char val;
   if( len > 2 ){
      len = 2;
   }
   val = rfm70_register_read( RFM70_REG_CONFIG );
   if( len == 0 ){
      val &= 0xF3;
      rfm70_register_write( RFM70_REG_EN_AA, 0 );
      rfm70_register_write( RFM70_REG_CONFIG, val );
   } else {
      rfm70_register_write( RFM70_REG_EN_AA, 0x3F );
      val &= 0xFB;
      if( val == 2 ){
         val |= 0x04;
      }
      rfm70_register_write( RFM70_REG_CONFIG, val );
   }
}

void rfm70_address_length( unsigned char len ){
   if( len > 5 ){
      len = 5;
   }
   if( len < 3 ){
      len = 3;
   }
   rfm70_register_write( RFM70_REG_SETUP_AW, len - 2  );
}

unsigned char rfm70_transmit_fifo_full( void ){
  unsigned char s;
  s = rfm70_register_read( RFM70_REG_FIFO_STATUS );
  return ( s & FIFO_STATUS_TX_FULL ) != 0;
}

unsigned char rfm70_receive_fifo_empty( void ){
  unsigned char s;
  s = rfm70_register_read( RFM70_REG_FIFO_STATUS );
  return ( s & FIFO_STATUS_RX_EMPTY ) != 0;
}

void rfm70_receive_address_p0( const unsigned char address[  ] ){
   rfm70_buffer_write( RFM70_REG_RX_ADDR_P0, address, 5 );
}

void rfm70_receive_address_p1( const unsigned char address[  ] ){
   rfm70_buffer_write( RFM70_REG_RX_ADDR_P1, address, 5 );
}

void rfm70_receive_address_pn( unsigned char channel, unsigned char address ){
   rfm70_register_write( RFM70_REG_RX_ADDR_P0 + channel, address );
}

void rfm70_transmit_address( const unsigned char *address ){
   rfm70_buffer_write( RFM70_REG_TX_ADDR, address, 5 );
}

unsigned char rfm70_retransmit_count( void ){
   return rfm70_register_read( RFM70_REG_OBSERVE_TX ) & 0x0F;
}

unsigned char rfm70_lost_packets_count( void ){
   return ( rfm70_register_read( RFM70_REG_OBSERVE_TX ) >> 4 ) & 0x0F;
}

void rfm70_pipe_autoack( unsigned char pipe, unsigned char enabled ){
   unsigned char val = rfm70_register_read( RFM70_REG_EN_AA );
   if( pipe > 5 ){
      pipe = 5;
   }
   if( enabled ){
      val |= 1 << pipe;
   } else {
      val &= ~ ( 1 << pipe );
   }
   rfm70_register_write( RFM70_REG_EN_AA, val );
}

void rfm70_pipe_enable( unsigned char pipe, unsigned char enabled ){
   unsigned char val = rfm70_register_read( RFM70_REG_EN_RXADDR );
   if( pipe > 5 ){
      pipe = 5;
   }
   if( enabled ){
      val |= 1 << pipe;
   } else {
      val &= ~ ( 1 << pipe );
   }
   rfm70_register_write( RFM70_REG_EN_RXADDR, val );
}

void rfm70_lost_packets_reset( void ){
   unsigned char val = rfm70_register_read( RFM70_REG_RF_CH );
   rfm70_register_write( RFM70_REG_RF_CH, val );
}

void rfm70_retransmit_delay_attempts( unsigned char d, unsigned char n ){
   rfm70_register_write(
      RFM70_REG_SETUP_RETR, ( n & 0x0F ) | (( d & 0x0F ) << 4 ));
}

void rfm70_lna_high( void ){
   unsigned char val = rfm70_register_read( RFM70_REG_RF_SETUP );
   val |= 0x01;
   rfm70_register_write( RFM70_REG_RF_SETUP, val );
}

void rfm70_lna_low( void ){
   unsigned char val = rfm70_register_read( RFM70_REG_RF_SETUP );
   val &= 0xFE;
   rfm70_register_write( RFM70_REG_RF_SETUP, val );
}

void rfm70_power( unsigned char level ){
   if( level > 3 ){
       level = 3;
   }
   CE = 0 ;
   unsigned char val = rfm70_register_read( RFM70_REG_RF_SETUP );
   val &= 0x09;
   val |= 0x30;
   val |= ( level << 1 );
   rfm70_register_write( RFM70_REG_RF_SETUP, val );
   CE = 1;
}

void rfm70_channel_payload_size( unsigned char channel, unsigned char size ){
   unsigned char val;
   if( size > 32 ){
      size = 32;
   }
   val = rfm70_register_read( RFM70_REG_DYNPD );
   if( size == 0 ){
      val |= 1 << channel;
   } else {
      val &= ~ ( 1 << channel );
   }
   rfm70_register_write( RFM70_REG_DYNPD, val );
   rfm70_register_write( RFM70_REG_RX_PW_P0 + channel, size );
}

void rfm70_transmit_message(
   const unsigned char *buf,
   unsigned char length
){
   if( length > 32 ){
      length = 32;
   }
   rfm70_buffer_write( RFM70_CMD_W_TX_PAYLOAD, buf, length );
}

void rfm70_transmit_message_once(
   const unsigned char *buf,
   unsigned char length
){
   if( length > 32 ){
      length = 32;
   }
   rfm70_buffer_write( RFM70_CMD_W_TX_PAYLOAD_NOACK, buf, length );
}

unsigned char rfm70_receive_next_pipe( void ){
   unsigned char status = rfm70_register_read( RFM70_REG_STATUS );
   return ( status >> 1 ) & 0x07;
}

unsigned char rfm70_receive_next_length( void ){
   return rfm70_register_read( RFM70_CMD_R_RX_PL_WID );
}

unsigned char rfm70_receive(
   unsigned char * pipe,
   unsigned char *buf,
   unsigned char * length
){
   unsigned char p = rfm70_receive_next_pipe();
   if( p == 0x07 ){
      return 0;
   }
   * pipe = p;
   * length = rfm70_receive_next_length();
   rfm70_buffer_read( RFM70_CMD_R_RX_PAYLOAD, buf, * length );
   return 1;
}


void rfm70_init( void ){
   unsigned char i;

   CE = ( 0 );
   CSN = (1);

   //RFM70_SCK( 0 );
   //RFM70_MOSI( 0 );

   // delay at least 50ms.
   // the example code says so, but why??
   __delay_ms(50);
   //wait_ms( 50 );

   // write array of default init settings
   rfm70_bank( 0 );
   for( i = 0; i < BANK0_ENTRIES; i++ ){
     rfm70_register_write( Bank0_Reg[ i ][0], Bank0_Reg[ i ][1] );
   }

   rfm70_receive_address_p0( RX0_Address );
   rfm70_receive_address_p1( RX0_Address );
   rfm70_transmit_address( RX0_Address );

   // enable the extra features
   i = rfm70_register_read(29);
   if( i == 0 ){ // only when the extra features are not yet activated!
      rfm70_register_write( RFM70_CMD_ACTIVATE, 0x73 );
   }

   // select dynamic payload length data pipe5\4\3\2\1\0
   rfm70_register_write( 28, 0x3F );

   // select Dynamic Payload Length, Payload with ACK, W_TX_PAYLOAD_NOACK
   rfm70_register_write( 29, 0x07 );

   // dynamic payload sizing on channels 0 and 1
   rfm70_channel_payload_size( 0, 0 );
   rfm70_channel_payload_size( 1, 0 );

   rfm70_init_bank1();
   //wait_ms ( 50 );
   __delay_ms(50);
   rfm70_mode_receive();
}
