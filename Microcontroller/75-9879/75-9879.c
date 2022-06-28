//;************************************************
//DESCRIPTION:
//SIMTEK PART # 10-9186-01
//75-8902
//10/12/2021
//David B.
//;************************************************
#include<absacc.h>
#include<aduc841.h>
#include<intrins.h>
//;************************************************
#define CONTROL_COMMAND 0x23			//
#define SWITCH_STATUS   0x24			//
#define OPTIONS   		0x25			//
#define PANEL_STATUS    0x26			//
#define ENC_POT_STATUS  0x28			//
#define ECHO_OPTION_ID  0x30			//
#define REALTIME_OPTION_ID  0x31		//
#define BRT_CTRL_OPTION_ID  0x32		//
//;************************************************
//BEZEL PUSH BUTTON ADDRESSES
//COUNT STARTS UPPER LEFT BEZEL
#define PB_41 0x41						//
#define PB_42 0x42						//
#define PB_43 0x43						//
#define PB_44 0x44						//
#define PB_45 0x45						//
#define PB_46 0x46						//
#define PB_47 0x47						//
#define PB_48 0x48						//
#define PB_49 0x49						//
#define PB_4A 0x4A						//
#define PB_4B 0x4B						//
#define PB_4C 0x4C						//
#define PB_4D 0x4D						//
#define PB_4E 0x4E						//
#define PB_4F 0x4F						//
#define PB_50 0x50						//
#define PB_51 0x51						//
#define BRT_PB 0x52						//
#define ENC_PB 0x53						//
#define POT_ID 0x56						//
#define ENC_ID 0x58						//
#define BUTTON_PRESSED 0x31				//
#define BUTTON_RELEASED 0x30			//
//;************************************************
#define DISPLAY_DIM_ID 0x5A				//
#define CONTRAST_ID    0x5C				//
#define BEZEL_DIM_ID   0x5E				//
#define SKIDBALL_ID    0x60				//
//;************************************************
unsigned char bdata Abyte;					//create a bit addressable byte
sbit Abyte0 = Abyte^0;
sbit Abyte1 = Abyte^1;
sbit Abyte2 = Abyte^2;
sbit Abyte3 = Abyte^3;
sbit Abyte4 = Abyte^4;
sbit Abyte5 = Abyte^5;
sbit Abyte6 = Abyte^6;
sbit Abyte7 = Abyte^7;
//;************************************************
unsigned char bdata Bbyte;					//create a bit addressable byte
sbit Bbyte0 = Bbyte^0;
sbit Bbyte1 = Bbyte^1;
sbit Bbyte2 = Bbyte^2;
sbit Bbyte3 = Bbyte^3;
sbit Bbyte4 = Bbyte^4;
sbit Bbyte5 = Bbyte^5;
sbit Bbyte6 = Bbyte^6;
sbit Bbyte7 = Bbyte^7;
//;************************************************
// AD7247A Control Lines				//
//;************************************************
sbit csa        = P3^2;					//                 
sbit csb        = P3^3;					//
sbit wr         = P3^4;					//
//;************************************************
// FPGA Reset							//
//;************************************************
sbit fpga_reset	= P3^5;					// u3 pin 27
//;************************************************
// MAX144ACUA Control Lines				//
//;************************************************
sbit ADC_clk    = P2^0;					//
sbit ADC_cs     = P2^1;					//
sbit ADC_data   = P2^3;					//
//;************************************************
// 21-500 Day/Night Control line		//
//;************************************************ 
sbit day_night  = P2^2;					// 
//;************************************************
bit	reset_bit           		= 0;				// reset flag
bit send_status_bit             = 0;
bit update_bezel_dimming_bit    = 0;
bit update_display_contrast_bit = 0;
bit update_display_dimming_bit  = 0;
bit update_skidball_bit         = 0;
bit send_single_switch_status   = 0;
bit send_encoder_status         = 0;
bit send_pot_status             = 0;
bit send_display_dim_values     = 0;
bit send_contrast_values        = 0;
bit send_bezel_dim_values       = 0;
bit send_skidball_values        = 0;
bit send_echo_value        		= 0;
bit send_realtime_value        	= 0;
bit send_lcd_value        		= 0;
//;************************************************
unsigned char RS_micro_firmware[] =  {0x75,0x89,0x02,0x2D};
unsigned char RS_fpga_firmware[4];		// array of 4 sequential storage locations 0 - 3
//;************************************************
unsigned char temptransmit;
unsigned char tempreceive;

unsigned char tempskidhi;
unsigned char tempskidlo;
//;************************************************
unsigned char display_dim_msn;
unsigned char display_dim_lsn;
unsigned char display_dim_byte;
unsigned char old_display_dim_byte;

unsigned char contrast_msn = 0x30;
unsigned char contrast_lsn = 0x35;
unsigned char contrast_byte;

unsigned char bezel_dim_msn = 0x30;
unsigned char bezel_dim_lsn = 0x30;
unsigned char bezel_dim_byte;

unsigned char skidball_msn = 0x37;
unsigned char skidball_lsn = 0x46;
unsigned char skidball_byte;
unsigned int  look_up_value;

unsigned char enc_msn = 0x30;
unsigned char enc_lsn = 0x30;
unsigned char enc_byte;

unsigned char brt_msn = 0x30;
unsigned char brt_lsn = 0x30;
unsigned char old_brt_byte; 
unsigned char new_brt_byte;
//;************************************************
unsigned char s_display_dim_msn = 0x30;
unsigned char s_display_dim_lsn = 0x30;

unsigned char s_contrast_msn = 0x30;
unsigned char s_contrast_lsn = 0x35;

unsigned char s_bezel_dim_msn = 0x30;
unsigned char s_bezel_dim_lsn = 0x30;

unsigned char s_skidball_msn = 0x37;
unsigned char s_skidball_lsn = 0x46;

unsigned char s_enc_msn = 0x30;
unsigned char s_enc_lsn = 0x30;
//;************************************************
unsigned char switch_id;
unsigned char echo = 0x30;						//;ECHO OFF IS DEFAULT
unsigned char realtime = 0x30;					//;REPORT ON CHANGE IS DEFAULT
unsigned char brt_control = 0x31;				//;SERIAL BRIGHTNESS CONTROL IS DEFAULT
unsigned char Brt_Knob;
//;************************************************
unsigned char KEYADDRESS;
unsigned char KEYDATA;
unsigned char SWOLD1;
unsigned char SWOLD2;
unsigned char SWOLD3;
unsigned char SWOLD4;
unsigned char SWOLD5;
//;************************************************
unsigned char PB_41_Status = 0x30;				//
unsigned char PB_42_Status = 0x30;				//
unsigned char PB_43_Status = 0x30;				//
unsigned char PB_44_Status = 0x30;				//
unsigned char PB_45_Status = 0x30;				//
unsigned char PB_46_Status = 0x30;				//
unsigned char PB_47_Status = 0x30;				//
unsigned char PB_48_Status = 0x30;				//
unsigned char PB_49_Status = 0x30;				//
unsigned char PB_4A_Status = 0x30;				//
unsigned char PB_4B_Status = 0x30;				//
unsigned char PB_4C_Status = 0x30;				//
unsigned char PB_4D_Status = 0x30;				//
unsigned char PB_4E_Status = 0x30;				//
unsigned char PB_4F_Status = 0x30;				//
unsigned char PB_50_Status = 0x30;				//
unsigned char PB_51_Status = 0x30;				//
unsigned char BRT_PB_Status = 0x30;				//
unsigned char ENC_PB_Status = 0x30;				//
unsigned char Switch_Data;						//
unsigned long int i;
unsigned long int j;
unsigned char difference;						//
//;************************************************
unsigned char code dim_value[256] =
{
//NVIS
0xFF,0xFD,0xFB,0xF9,0xF7,0xF5,0xF3,0xF1,0xEF,0xED,0xEB,0xE9,0xE7,0xE5,0xE3,0xE1,
0xDF,0xDD,0xDB,0xD9,0xD7,0xD5,0xD3,0xD1,0xCF,0xCD,0xCB,0xC9,0xC7,0xC5,0xC3,0xC1,
0xBF,0xBD,0xBB,0xB9,0xB7,0xB5,0xB3,0xB1,0xAF,0xAD,0xAB,0xA9,0xA7,0xA5,0xA3,0xA1,
0x9F,0x9D,0x9B,0x99,0x97,0x95,0x93,0x91,0x8F,0x8D,0x8B,0x89,0x87,0x85,0x83,0x81,
0x7F,0x7D,0x7B,0x79,0x77,0x75,0x73,0x71,0x6F,0x6D,0x6B,0x69,0x67,0x65,0x63,0x61,
0x5F,0x5D,0x5B,0x59,0x57,0x55,0x53,0x51,0x4F,0x4D,0x4B,0x49,0x47,0x45,0x43,0x41,
0x3F,0x3D,0x3B,0x39,0x37,0x35,0x33,0x31,0x2F,0x2D,0x2B,0x29,0x27,0x25,0x23,0x21,
0x1F,0x1D,0x1B,0x19,0x17,0x15,0x13,0x11,0x0F,0x0D,0x0B,0x09,0x07,0x05,0x03,0x01,
//NON NVIS
0xFB,0xFA,0xF9,0xF8,0xF7,0xF5,0xF3,0xF1,0xEF,0xED,0xEB,0xE9,0xE7,0xE5,0xE3,0xE1,
0xDF,0xDD,0xDB,0xD9,0xD7,0xD5,0xD3,0xD1,0xCF,0xCD,0xCB,0xC9,0xC7,0xC5,0xC3,0xC1,
0xBF,0xBD,0xBB,0xB9,0xB7,0xB5,0xB3,0xB1,0xAF,0xAD,0xAB,0xA9,0xA7,0xA5,0xA3,0xA1,
0x9F,0x9D,0x9B,0x99,0x97,0x95,0x93,0x91,0x8F,0x8D,0x8B,0x89,0x87,0x85,0x83,0x81,
0x7F,0x7D,0x7B,0x79,0x77,0x75,0x73,0x71,0x6F,0x6D,0x6B,0x69,0x67,0x65,0x63,0x61,
0x5F,0x5D,0x5B,0x59,0x57,0x55,0x53,0x51,0x4F,0x4D,0x4B,0x49,0x47,0x45,0x43,0x41,
0x3F,0x3D,0x3B,0x39,0x37,0x35,0x33,0x31,0x2F,0x2D,0x2B,0x29,0x27,0x25,0x23,0x21,
0x1F,0x1E,0x1D,0x1C,0x1B,0x1A,0x19,0x18,0x17,0x16,0x15,0x14,0x13,0x12,0x11,0x10,
//0x1F,0x1D,0x1B,0x19,0x17,0x15,0x13,0x11,0x0F,0x0D,0x0B,0x09,0x07,0x05,0x03,0x01,
};
/*//;************************************************
unsigned int code skid_value[256] =
{
0x0000,0x0010,0x0020,0x0030,0x0040,0x0050,0x0060,0x0070,0x0080,0x0090,0x00A0,0x00B0,0x00C0,0x00D0,0x00E0,0x00F0,
0x0100,0x0110,0x0120,0x0130,0x0140,0x0150,0x0160,0x0170,0x0180,0x0190,0x01A0,0x01B0,0x01C0,0x01D0,0x01E0,0x01F0,
0x0200,0x0210,0x0220,0x0230,0x0240,0x0250,0x0260,0x0270,0x0280,0x0290,0x02A0,0x02B0,0x02C0,0x02D0,0x02E0,0x02F0,
0x0300,0x0310,0x0320,0x0330,0x0340,0x0350,0x0360,0x0370,0x0380,0x0390,0x03A0,0x03B0,0x03C0,0x03D0,0x03E0,0x03F0,
0x0400,0x0410,0x0420,0x0430,0x0440,0x0450,0x0460,0x0470,0x0480,0x0490,0x04A0,0x04B0,0x04C0,0x04D0,0x04E0,0x04F0,
0x0500,0x0510,0x0520,0x0530,0x0540,0x0550,0x0560,0x0570,0x0580,0x0590,0x05A0,0x05B0,0x05C0,0x05D0,0x05E0,0x05F0,
0x0600,0x0610,0x0620,0x0630,0x0640,0x0650,0x0660,0x0670,0x0680,0x0690,0x06A0,0x06B0,0x06C0,0x06D0,0x06E0,0x06F0,
0x0700,0x0710,0x0720,0x0730,0x0740,0x0750,0x0760,0x0770,0x0780,0x0790,0x07A0,0x07B0,0x07C0,0x07D0,0x07E0,0x07F0,
0x0800,0x0810,0x0820,0x0830,0x0840,0x0850,0x0860,0x0870,0x0880,0x0890,0x08A0,0x08B0,0x08C0,0x08D0,0x08E0,0x08F0,
0x0900,0x0910,0x0920,0x0930,0x0940,0x0950,0x0960,0x0970,0x0980,0x0990,0x09A0,0x09B0,0x09C0,0x09D0,0x09E0,0x09F0,
0x0A00,0x0A10,0x0A20,0x0A30,0x0A40,0x0A50,0x0A60,0x0A70,0x0A80,0x0A90,0x0AA0,0x0AB0,0x0AC0,0x0AD0,0x0AE0,0x0AF0,
0x0B00,0x0B10,0x0B20,0x0B30,0x0B40,0x0B50,0x0B60,0x0B70,0x0B80,0x0B90,0x0BA0,0x0BB0,0x0BC0,0x0BD0,0x0BE0,0x0BF0,
0x0C00,0x0C10,0x0C20,0x0C30,0x0C40,0x0C50,0x0C60,0x0C70,0x0C80,0x0C90,0x0CA0,0x0CB0,0x0CC0,0x0CD0,0x0CE0,0x0CF0,
0x0D00,0x0D10,0x0D20,0x0D30,0x0D40,0x0D50,0x0D60,0x0D70,0x0D80,0x0D90,0x0DA0,0x0DB0,0x0DC0,0x0DD0,0x0DE0,0x0DF0,
0x0E00,0x0E10,0x0E20,0x0E30,0x0E40,0x0E50,0x0E60,0x0E70,0x0E80,0x0E90,0x0EA0,0x0EB0,0x0EC0,0x0ED0,0x0EE0,0x0EF0,
0x0F00,0x0F10,0x0F20,0x0F30,0x0F40,0x0F50,0x0F60,0x0F70,0x0F80,0x0F90,0x0FA0,0x0FB0,0x0FC0,0x0FD0,0x0FE0,0x0FF0,
};
*/
unsigned int code skid_value[256] =
{
//-5VDC TO +5VDC
0x0400,0x0408,0x0410,0x0418,0x0420,0x0428,0x0430,0x0438,0x0440,0x0448,0x0450,0x0458,0x0460,0x0468,0x0470,0x0478,
0x0480,0x0488,0x0490,0x0498,0x04A0,0x04A8,0x04B0,0x04B8,0x04C0,0x04C8,0x04D0,0x04D8,0x04E0,0x04E8,0x04F0,0x04F8,
0x0500,0x0508,0x0510,0x0518,0x0520,0x0528,0x0530,0x0538,0x0540,0x0548,0x0550,0x0558,0x0560,0x0568,0x0570,0x0578,
0x0580,0x0588,0x0590,0x0598,0x05A0,0x05A8,0x05B0,0x05B8,0x05C0,0x05C8,0x05D0,0x05D8,0x05E0,0x05E8,0x05F0,0x05F8,
0x0600,0x0608,0x0610,0x0618,0x0620,0x0628,0x0630,0x0638,0x0640,0x0648,0x0650,0x0658,0x0660,0x0668,0x0670,0x0678,
0x0680,0x0688,0x0690,0x0698,0x06A0,0x06A8,0x06B0,0x06B8,0x06C0,0x06C8,0x06D0,0x06D8,0x06E0,0x06E8,0x06F0,0x06F8,
0x0700,0x0708,0x0710,0x0718,0x0720,0x0728,0x0730,0x0738,0x0740,0x0748,0x0750,0x0758,0x0760,0x0768,0x0770,0x0778,
0x0780,0x0788,0x0790,0x0798,0x07A0,0x07A8,0x07B0,0x07B8,0x07C0,0x07C8,0x07D0,0x07D8,0x07E0,0x07E8,0x07F0,0x07F8,
0x0800,0x0808,0x0810,0x0818,0x0820,0x0828,0x0830,0x0838,0x0840,0x0848,0x0850,0x0858,0x0860,0x0868,0x0870,0x0878,
0x0880,0x0888,0x0890,0x0898,0x08A0,0x08A8,0x08B0,0x08B8,0x08C0,0x08C8,0x08D0,0x08D8,0x08E0,0x08E8,0x08F0,0x08F8,
0x0900,0x0908,0x0910,0x0918,0x0920,0x0928,0x0930,0x0938,0x0940,0x0948,0x0950,0x0958,0x0960,0x0968,0x0970,0x0978,
0x0980,0x0988,0x0990,0x0998,0x09A0,0x09A8,0x09B0,0x09B8,0x09C0,0x09C8,0x09D0,0x09D8,0x09E0,0x09E8,0x09F0,0x09F8,
0x0A00,0x0A08,0x0A10,0x0A18,0x0A20,0x0A28,0x0A30,0x0A38,0x0A40,0x0A48,0x0A50,0x0A58,0x0A60,0x0A68,0x0A70,0x0A78,
0x0A80,0x0A88,0x0A90,0x0A98,0x0AA0,0x0AA8,0x0AB0,0x0AB8,0x0AC0,0x0AC8,0x0AD0,0x0AD8,0x0AE0,0x0AE8,0x0AF0,0x0AF8,
0x0B00,0x0B08,0x0B10,0x0B18,0x0B20,0x0B28,0x0B30,0x0B38,0x0B40,0x0B48,0x0B50,0x0B58,0x0B60,0x0B68,0x0B70,0x0B78,
0x0B80,0x0B88,0x0B90,0x0B98,0x0BA0,0x0BA8,0x0BB0,0x0BB8,0x0BC0,0x0BC8,0x0BD0,0x0BD8,0x0BE0,0x0BE8,0x0BF0,0x0BF8,

};
//;************************************************
unsigned char code contrast_value[16] =
{
0x00,0x17,0x27,0x37,0x47,0x57,0x67,0x77,0x87,0x97,0xA7,0xB7,0xC7,0xD7,0xE7,0xFF,
};
//;************************************************
void DELAY_LOOP_Wait(const unsigned int DELAY);
void setup_serial(void);
void setup_pwm(void);
void setup_dac(void);
void reset_everything(void);
void transmit_data(void);
void receive_data(void);
void update_dimming(void);
void send_status(void);
void send_single_sw_status(void);
void switch_send(void);
void convert_to_transmit(void);
void convert_to_receive(void);
void update_dac(void);
void update_skidball(void);
void encoder(void);
void clk_adc(void);
void adc_read(void);
void average_adc(void);
void row_1(void);
void row_2(void);
void row_3(void);
void row_4(void);
void row_5(void);
void send_command_values(void);
void send_option_values(void);
//;************************************************
void main(void)
{	setup_serial();
	setup_dac();
	setup_pwm();
	ewait = 0x07;	
	fpga_reset = 1;
	fpga_reset = 0;
	reset_bit = 1;
	reset_everything();
	P1 	= 0x00;							// configure as inputs 
	day_night = 0; 
	PBYTE[0x82] = 0x01; 
	j = 0;
	difference = 0;
	i = 0;
	update_display_contrast_bit = 1; 
//;************************************************
	while(1)						
	{
//	reset_everything();
//	update_skidball();//;--------------------------
	
	update_dimming();
	update_skidball();//;--------------------------
	
	send_status();
	update_skidball();//;--------------------------
	
	send_single_sw_status();
	update_skidball();//;--------------------------
	
	update_dac();	
	update_skidball();//;--------------------------	
	
	encoder();
	update_skidball();//;--------------------------
	
	row_1();
	update_skidball();//;--------------------------
	
	row_2();
	update_skidball();//;--------------------------
	
	row_3();
	update_skidball();//;--------------------------
	
	row_4();
	update_skidball();//;--------------------------
	
	row_5();
	update_skidball();//;--------------------------
	
	send_command_values();
	update_skidball();//;--------------------------
	
	send_option_values();
	update_skidball();//;--------------------------

	adc_read();
	update_skidball();//;--------------------------
	
	i++;
	if(i == 100)
	{
	average_adc();
	j = 0;
	difference = 0;
	i = 0;
	}
	}
}
//;************************************************
void DELAY_LOOP_Wait(const unsigned int DELAY)
{unsigned int x, y;
for (x = 0; x <= DELAY; x++)
{for (y = 0; y <= 255; y++);}}			// DELAY_LOOP_Wait(255);
//;************************************************
void setup_serial(void)
{ 	
	SCON	= 0x40;						// MODE1 UART
	T3CON	= 0x84;						// 19200K BPS	
	T3FD	= 0x01;						// DEFAULT IS 9600K BAUD
	REN 	= 1;						// RECEIVE ENABLED
	ES 		= 1;						// ENABLE SERIAL
	EA		= 1;						// ALL INTERRUPTS OK
	RI 		= 0;						// CLR RECEIVE FLAG
	TI		= 0;						// CLR TRANSMIT FLAG
}
//;************************************************
void setup_pwm(void)
{
 	PWMCON = 0x5F;						//
 	PWM1L  = 0xFF;						// p2.6 resolution
 	PWM1H  = 0xFF;						// p2.7 resolution
 	PWM0L  = 0xFF;						// PWM0 DUTY CYCLE OUTPUT P2.6
 	PWM0H  = 0xFF;						// PWM1 DUTY CYCLE OUTPUT P2.7
}
//;************************************************
void setup_dac(void)
{
	ADCCON1 = 0x80;						// TURN ADC ON / ADC HAS TO BE ON FOR DAC TO WORK
	DACCON  = 0xFF; 					// SET DAC TO 8 BIT / 0 TO +3.3VDC AND TURN DAC0 ON
	DAC0L   = 0x00;  					// RESET TO 0
	DAC0H   = 0x00;  					// RESET TO 0
	DAC1L   = 0x00;  					// RESET TO 0
	DAC1H   = 0x00;  					// RESET TO 0
}
//;************************************************
void reset_everything(void)
{
if(reset_bit)							// check to see if reset flag is set
	{
	display_dim_byte = 0x00;
									   	
	PWM0L = 0xFF;
	PWM0H = 0xFF;						// reset display dimming

	DAC0L   = 0x00;  					// RESET TO 0
	DAC1L   = 0x87;  					// SET TO MIDSCALE

	PBYTE[0x80] = 0xFF;					// DAC_Data_Out(7 downto 0) <= AddrData;
	PBYTE[0x81] = 0x07;					// DAC_Data_Out(11 downto 8)

	csa = 0;
	wr  = 0;
	wr  = 1;
	csa = 1;
	
	RS_fpga_firmware[0] = PBYTE[2];
	RS_fpga_firmware[1] = PBYTE[3];
	RS_fpga_firmware[2] = PBYTE[4];
	RS_fpga_firmware[3] = PBYTE[5];

	reset_bit = 0;
    }
}
//;************************************************
void update_dac(void)
{
if(update_display_contrast_bit)
	{

	s_contrast_msn = contrast_msn; 
	s_contrast_lsn = contrast_lsn;

	tempreceive = contrast_msn;
	convert_to_receive();
	contrast_msn = tempreceive;
    
	tempreceive = contrast_lsn;
    convert_to_receive();
	contrast_lsn = tempreceive; 
   	
	contrast_msn = (contrast_msn << 4) & 0xF0;
   	contrast_lsn = contrast_lsn & 0x0F;
   	
	contrast_byte = contrast_msn | contrast_lsn;

	DAC1L = contrast_value[contrast_byte];
//;************************************************    
	if(echo == 0x31)								//; send contrast echo
	{
  	SBUF = CONTROL_COMMAND;
	transmit_data();
  	SBUF = CONTRAST_ID;
	transmit_data();
	SBUF = s_contrast_msn;
	transmit_data();
  	SBUF = s_contrast_lsn;
	transmit_data();
  	SBUF = 0x0D;
	transmit_data();
  	SBUF = 0x0A;
	transmit_data();
	}  
//;************************************************
	update_display_contrast_bit = 0;
//;************************************************
	}
}
//;************************************************
void update_skidball(void)
{
if(update_skidball_bit)
{
	s_skidball_msn = skidball_msn; 
	s_skidball_lsn = skidball_lsn;

    tempreceive = skidball_msn;
	convert_to_receive();
	skidball_msn = tempreceive;
    
	tempreceive = skidball_lsn;
    convert_to_receive();
	skidball_lsn = tempreceive; 
   	
	skidball_msn = (skidball_msn << 4) & 0xF0;
   	skidball_lsn = skidball_lsn & 0x0F;
   	
	skidball_byte = skidball_msn | skidball_lsn;

	look_up_value = skid_value[skidball_byte];

	tempskidlo = (look_up_value & 0xFF);
	tempskidhi = (look_up_value >> 8) & 0x0F;

	PBYTE[0x80] = tempskidlo;						//; DAC_Data_Out(7 downto 0) <= AddrData;
	PBYTE[0x81] = tempskidhi;						//; DAC_Data_Out(11 downto 8)

	csa = 0;
	wr  = 0;
	wr  = 1;
	csa = 1;
//;************************************************    
	if(echo == 0x31)								//; send skidball echo
	{
  	SBUF = CONTROL_COMMAND;
	transmit_data();
  	SBUF = SKIDBALL_ID;
	transmit_data();
	SBUF = s_skidball_msn;
	transmit_data();
  	SBUF = s_skidball_lsn;
	transmit_data();
  	SBUF = 0x0D;
	transmit_data();
  	SBUF = 0x0A;
	transmit_data();
	}
//;************************************************	
	update_skidball_bit = 0;
//;************************************************
	}
}
//;************************************************
void encoder(void)
{
	Abyte = PBYTE[0x35];
	if(Abyte != enc_byte)
	{
	enc_byte = Abyte;

	enc_msn = (enc_byte >> 4) & 0x0F;
   	enc_lsn = enc_byte & 0x0F;    
	
	temptransmit = enc_msn;
	convert_to_transmit();
	enc_msn = temptransmit;

	temptransmit = enc_lsn;
	convert_to_transmit();
	enc_lsn = temptransmit;
	
if(realtime == 0x30)
	{
   	SBUF = ENC_POT_STATUS;		//;0x28
	transmit_data();
	SBUF = ENC_ID;				//;0x58				
	transmit_data();
	SBUF = enc_msn;				//;0-9,A-F	
	transmit_data();
	SBUF = enc_lsn;				//;0-9,A-F	
	transmit_data();
	SBUF = 0x0D;				//;Return	
	transmit_data();
	SBUF = 0x0A;				//;New Line	
	transmit_data();
	}
	}
if(send_encoder_status)
    {
   	SBUF = ENC_POT_STATUS;		//;0x28
	transmit_data();
	SBUF = ENC_ID;				//;0x58				
	transmit_data();
	SBUF = enc_msn;				//;0-9,A-F	
	transmit_data();
	SBUF = enc_lsn;				//;0-9,A-F	
	transmit_data();
	SBUF = 0x0D;				//;Return	
	transmit_data();
	SBUF = 0x0A;				//;New Line	
	transmit_data();
	send_encoder_status = 0;
	
	if(echo == 0x31)								//; send encoder echo
	{
	DELAY_LOOP_Wait(100);
   	SBUF = ENC_POT_STATUS;		//;0x28
	transmit_data();
	SBUF = ENC_ID;				//;0x58				
	transmit_data();
	SBUF = 0x0D;				//;Return	
	transmit_data();
	SBUF = 0x0A;				//;New Line	
	transmit_data();
	}	
	}
}
//;************************************************
void keyboardsend(void)
{if(realtime == 0x30)
	{
  	SBUF = SWITCH_STATUS;
	transmit_data();
  	SBUF = KEYADDRESS;
	transmit_data();
  	SBUF = KEYDATA;
	transmit_data();
  	SBUF = 0x0D;
	transmit_data();
  	SBUF = 0x0A;
	transmit_data();	
	}
}
//;************************************************
void row_1(void)
{unsigned int i;
 	for(i = 0; i <= 8; i++)				// LOAD DEBOUNCE COUNT
 		{Abyte = PBYTE[0x30];				// 
	 	 Abyte = ~Abyte;					// COMPLIMENT THE DATA IN ABYTE
	 	 Bbyte = Abyte;}					// STORE A COPY OF IT IN Bbyte
	if(Abyte != SWOLD1)						// has there been a change	
	 {Abyte = Abyte ^ SWOLD1;				// XOR new value with old to see what changed
	 	if(Abyte != 0)						//
	 	{SWOLD1 = Bbyte;}					// change detected store new value for comparison next time around
//41									
	  if(Abyte0 != 1)goto CHK_42;
	  if(Bbyte0 == 1) 						
	  {
	   KEYADDRESS = PB_41;
	   KEYDATA = BUTTON_PRESSED;
	   PB_41_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte0 == 0) 						
	  {
	   KEYADDRESS = PB_41;
	   KEYDATA = BUTTON_RELEASED;
	   PB_41_Status = BUTTON_RELEASED;  						
	   keyboardsend();}					
CHK_42:									
	  if(Abyte1 != 1)goto CHK_43;
	  if(Bbyte1 == 1) 						
	  {
	   KEYADDRESS = PB_42;
	   KEYDATA = BUTTON_PRESSED;
	   PB_42_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte1 == 0) 						
	  {
	   KEYADDRESS = PB_42;
	   KEYDATA = BUTTON_RELEASED;
	   PB_42_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
CHK_43:									
	  if(Abyte2 != 1)goto CHK_44;
	  if(Bbyte2 == 1) 						
	  {
	   KEYADDRESS = PB_43;
	   KEYDATA = BUTTON_PRESSED;
	   PB_43_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte2 == 0) 						
	  {
	   KEYADDRESS = PB_43;
	   KEYDATA = BUTTON_RELEASED;
	   PB_43_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
CHK_44:									
	  if(Abyte3 != 1)goto CHK_45;
	  if(Bbyte3 == 1) 						
	  {
	   KEYADDRESS = PB_44;
	   KEYDATA = BUTTON_PRESSED;
	   PB_44_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte3 == 0) 						
	  {
	   KEYADDRESS = PB_44;
	   KEYDATA = BUTTON_RELEASED;
	   PB_44_Status = BUTTON_RELEASED; 						
	   keyboardsend();}		
CHK_45:									
	  if(Abyte4 != 1)goto CHK_46;
	  if(Bbyte4 == 1) 						
	  {
	   KEYADDRESS = PB_45;
	   KEYDATA = BUTTON_PRESSED;
	   PB_45_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte4 == 0) 						
	  {
	   KEYADDRESS = PB_45;
	   KEYDATA = BUTTON_RELEASED;
	   PB_45_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
CHK_46:									
	  if(Abyte5 != 1)return;
	  if(Bbyte5 == 1) 						
	  {
	   KEYADDRESS = PB_46;
	   KEYDATA = BUTTON_PRESSED;
	   PB_46_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte5 == 0) 						
	  {
	   KEYADDRESS = PB_46;
	   KEYDATA = BUTTON_RELEASED;
	   PB_46_Status = BUTTON_RELEASED; 						
	   keyboardsend();}		
	}										
}
//;************************************************
void row_2(void)
{unsigned int i;
 	for(i = 0; i <= 8; i++)				// LOAD DEBOUNCE COUNT
 		{Abyte = PBYTE[0x31];				// 
	 	 Abyte = ~Abyte;					// COMPLIMENT THE DATA IN ABYTE
	 	 Bbyte = Abyte;}					// STORE A COPY OF IT IN Bbyte
	if(Abyte != SWOLD2)						// has there been a change	
	 {Abyte = Abyte ^ SWOLD2;				// XOR new value with old to see what changed
	 	if(Abyte != 0)						//
	 	{SWOLD2 = Bbyte;}					// change detected store new value for comparison next time around
//47									
	  if(Abyte0 != 1)goto CHK_48;
	  if(Bbyte0 == 1) 						
	  {
	   KEYADDRESS = PB_47;
	   KEYDATA = BUTTON_PRESSED;
	   PB_47_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte0 == 0) 						
	  {
	   KEYADDRESS = PB_47;
	   KEYDATA = BUTTON_RELEASED; 
	   PB_47_Status = BUTTON_RELEASED;						
	   keyboardsend();}					
CHK_48:									
	  if(Abyte1 != 1)goto CHK_49;
	  if(Bbyte1 == 1) 						
	  {
	   KEYADDRESS = PB_48;
	   KEYDATA = BUTTON_PRESSED;
	   PB_48_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte1 == 0) 						
	  {
	   KEYADDRESS = PB_48;
	   KEYDATA = BUTTON_RELEASED;
	   PB_48_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
CHK_49:									
	  if(Abyte2 != 1)goto CHK_4A;
	  if(Bbyte2 == 1) 						
	  {
	   KEYADDRESS = PB_49;
	   KEYDATA = BUTTON_PRESSED;
	   PB_49_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte2 == 0) 						
	  {
	   KEYADDRESS = PB_49;
	   KEYDATA = BUTTON_RELEASED;
	   PB_49_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
CHK_4A:									
	  if(Abyte3 != 1)goto CHK_4B;
	  if(Bbyte3 == 1) 						
	  {
	   KEYADDRESS = PB_4A;
	   KEYDATA = BUTTON_PRESSED;
	   PB_4A_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte3 == 0) 						
	  {
	   KEYADDRESS = PB_4A;
	   KEYDATA = BUTTON_RELEASED;
	   PB_4A_Status = BUTTON_RELEASED; 						
	   keyboardsend();}		
CHK_4B:									
	  if(Abyte4 != 1)return;
	  if(Bbyte4 == 1) 						
	  {
	   KEYADDRESS = PB_4B;
	   KEYDATA = BUTTON_PRESSED;
	   PB_4B_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte4 == 0) 						
	  {
	   KEYADDRESS = PB_4B;
	   KEYDATA = BUTTON_RELEASED;
	   PB_4B_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
	}										
}
//;************************************************
void row_3(void)
{unsigned int i;
 	for(i = 0; i <= 8; i++)				// LOAD DEBOUNCE COUNT
 		{Abyte = PBYTE[0x32];				// 
	 	 Abyte = ~Abyte;					// COMPLIMENT THE DATA IN ABYTE
	 	 Bbyte = Abyte;}					// STORE A COPY OF IT IN Bbyte
	if(Abyte != SWOLD3)						// has there been a change	
	 {Abyte = Abyte ^ SWOLD3;				// XOR new value with old to see what changed
	 	if(Abyte != 0)						//
	 	{SWOLD3 = Bbyte;}					// change detected store new value for comparison next time around
//4C									
	  if(Abyte0 != 1)goto CHK_4D;
	  if(Bbyte0 == 1) 						
	  {
	   KEYADDRESS = PB_4C;
	   KEYDATA = BUTTON_PRESSED;
	   PB_4C_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte0 == 0) 						
	  {
	   KEYADDRESS = PB_4C;
	   KEYDATA = BUTTON_RELEASED;
	   PB_4C_Status = BUTTON_RELEASED; 						
	   keyboardsend();}					
CHK_4D:									
	  if(Abyte1 != 1)goto CHK_4E;
	  if(Bbyte1 == 1) 						
	  {
	   KEYADDRESS = PB_4D;
	   KEYDATA = BUTTON_PRESSED;
	   PB_4D_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte1 == 0) 						
	  {
	   KEYADDRESS = PB_4D;
	   KEYDATA = BUTTON_RELEASED;
	   PB_4D_Status = BUTTON_RELEASED;  						
	   keyboardsend();}	
CHK_4E:									
	  if(Abyte2 != 1)goto CHK_4F;
	  if(Bbyte2 == 1) 						
	  {
	   KEYADDRESS = PB_4E;
	   KEYDATA = BUTTON_PRESSED;
	   PB_4E_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte2 == 0) 						
	  {
	   KEYADDRESS = PB_4E;
	   KEYDATA = BUTTON_RELEASED;
	   PB_4E_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
CHK_4F:									
	  if(Abyte3 != 1)goto CHK_50;
	  if(Bbyte3 == 1) 						
	  {
	   KEYADDRESS = PB_4F;
	   KEYDATA = BUTTON_PRESSED;
	   PB_4F_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte3 == 0) 						
	  {
	   KEYADDRESS = PB_4F;
	   KEYDATA = BUTTON_RELEASED;
	   PB_4F_Status = BUTTON_RELEASED; 						
	   keyboardsend();}		
CHK_50:									
	  if(Abyte4 != 1)goto CHK_51;
	  if(Bbyte4 == 1) 						
	  {
	   KEYADDRESS = PB_50;
	   KEYDATA = BUTTON_PRESSED;
	   PB_50_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte4 == 0) 						
	  {
	   KEYADDRESS = PB_50;
	   KEYDATA = BUTTON_RELEASED;
	   PB_50_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
CHK_51:									
	  if(Abyte5 != 1)return;
	  if(Bbyte5 == 1) 						
	  {
	   KEYADDRESS = PB_51;
	   KEYDATA = BUTTON_PRESSED;
	   PB_51_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte5 == 0) 						
	  {
	   KEYADDRESS = PB_51;
	   KEYDATA = BUTTON_RELEASED; 
	   PB_51_Status = BUTTON_RELEASED;						
	   keyboardsend();}		
	}										
}
//;************************************************
void row_4(void)
{unsigned int i;
 	for(i = 0; i <= 8; i++)				// LOAD DEBOUNCE COUNT
 		{Abyte = PBYTE[0x33];				// 
	 	 Abyte = ~Abyte;					// COMPLIMENT THE DATA IN ABYTE
	 	 Bbyte = Abyte;}					// STORE A COPY OF IT IN Bbyte
	if(Abyte != SWOLD4)						// has there been a change	
	 {Abyte = Abyte ^ SWOLD4;				// XOR new value with old to see what changed
	 	if(Abyte != 0)						//
	 	{SWOLD4 = Bbyte;}					// change detected store new value for comparison next time around
//52									
	  if(Abyte0 != 1)return;
	  if(Bbyte0 == 1) 						
	  {
	   KEYADDRESS = BRT_PB;
	   KEYDATA = BUTTON_PRESSED;
	   BRT_PB_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte0 == 0) 						
	  {
	   KEYADDRESS = BRT_PB;
	   KEYDATA = BUTTON_RELEASED;
	   BRT_PB_Status = BUTTON_RELEASED; 						
	   keyboardsend();}	
	  }
}
//;************************************************
void row_5(void)
{unsigned int i;
 	for(i = 0; i <= 8; i++)				// LOAD DEBOUNCE COUNT
 		{Abyte = PBYTE[0x34];				// 
	 	 Abyte = ~Abyte;					// COMPLIMENT THE DATA IN ABYTE
	 	 Bbyte = Abyte;}					// STORE A COPY OF IT IN Bbyte
	if(Abyte != SWOLD5)						// has there been a change	
	 {Abyte = Abyte ^ SWOLD5;				// XOR new value with old to see what changed
	 	if(Abyte != 0)						//
	 	{SWOLD5 = Bbyte;}					// change detected store new value for comparison next time around
//53									
	  if(Abyte0 != 1)return;
	  if(Bbyte0 == 1) 						
	  {
	   KEYADDRESS = ENC_PB;
	   KEYDATA = BUTTON_PRESSED;
	   ENC_PB_Status = BUTTON_PRESSED;						
	   keyboardsend();}
	  if(Bbyte0 == 0) 						
	  {
	   KEYADDRESS = ENC_PB;
	   KEYDATA = BUTTON_RELEASED;
	   ENC_PB_Status = BUTTON_RELEASED;  						
	   keyboardsend();}	
	  }
}
//;************************************************
void transmit_data(void)
{
	while(!TI);		
	TI = 0;
}
//;************************************************
void receive_data(void)
{
	while(!RI);		
	RI = 0;
}
//;************************************************
void convert_to_receive(void)
{
      if (tempreceive < 0x41)
      tempreceive = (tempreceive - 0x30);
      else
      tempreceive = (tempreceive - 0x37);
}
//;************************************************
void convert_to_transmit(void)
{
      if (temptransmit < 0x0A)
      temptransmit = (temptransmit + 0x30);
      else
      temptransmit = (temptransmit + 0x37);
}
//;************************************************
void clk_adc(void)
{
	ADC_clk = 1;
  	ADC_clk = 0;
}
//;************************************************
void adc_read(void)
{	
//unsigned long int i,j;
	//j = 0;
	//difference = 0;
	
	//for(i = 0; i < 256; i++)
	//{
	ADC_clk = 1;									// adc clock high
    ADC_cs = 1;										// adc cs high
    ADC_cs = 0;										// adc chip select lo
while(ADC_data == 0);								// wait for end of conversion
		clk_adc();									//
		clk_adc();									//
		clk_adc();									//
		clk_adc();									// channel id
		Abyte = 0x00;
		clk_adc();
	 	Abyte7 = ADC_data;		
		clk_adc();
	 	Abyte6 = ADC_data;
		clk_adc();
	 	Abyte5 = ADC_data;		
		clk_adc();
	 	Abyte4 = ADC_data;
		clk_adc();
		Abyte3 = ADC_data;		
		clk_adc(); 
	 	Abyte2 = ADC_data;
		clk_adc(); 
		Abyte1 = ADC_data;
		clk_adc(); 
		Abyte0 = ADC_data;		
		clk_adc();									// d3		
		clk_adc();									// d2		
		clk_adc();									// d1
		clk_adc();									// d0
		j += Abyte;
	//}
}
//;************************************************
void average_adc(void)
{
//	j = (j >> 8);								// divide by ?
	j = (j / 100);
	Abyte = j;									// new adc value
	new_brt_byte = Abyte;
//;************************************************	
	if(new_brt_byte != old_brt_byte)
	{
	Bbyte = new_brt_byte;							// old value to B
	Bbyte -= old_brt_byte;							// subtract new from old

//;************************************************
	if(Bbyte7 == 0) 							// difference is positive value
	{
	difference = Bbyte;
	}
//;************************************************	  	
	if(Bbyte7 == 1) 							// negative value convert 2's complement
	{									
	Bbyte = ~Bbyte;							// begin 2's complement conversion
	Bbyte = Bbyte + 1;						// add 1 to finsish process
	difference = Bbyte;
	}
//;************************************************				
	if(difference > 0x01) 					// update pwm if change is greater than 1xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx		
	{
//;************************************************	
	if(brt_control == 0x30)
	{
//;************************************************	
	if(new_brt_byte == 0x00)
	{PBYTE[0x82] = 0x00;}
	else
	{PBYTE[0x82] = 0x01;}
//;************************************************	
	if(new_brt_byte < 0x80)
	{
	day_night = 0;
	PWM0L = dim_value[new_brt_byte];
	PWM0H = ~bezel_dim_byte;		
	}
//;************************************************
if((old_brt_byte < 0x80) && (new_brt_byte > 0x7F))
	{   
    PBYTE[0x82] = 0x00;
	PWM0L = dim_value[new_brt_byte];	
	PWM0H = ~bezel_dim_byte;
	DELAY_LOOP_Wait(25);
	day_night = 1;
    DELAY_LOOP_Wait(25);
	PBYTE[0x82] = 0x01;
	}							 
//;************************************************	
	if(Abyte > 0x7F)
	{
	day_night = 1;
	PWM0L = dim_value[new_brt_byte];		
	PWM0H = ~bezel_dim_byte;		
	}
	}
	//}
//;************************************************
	brt_msn = (new_brt_byte >> 4) & 0x0F;
   	brt_lsn = new_brt_byte & 0x0F;    
	
	temptransmit = brt_msn;
	convert_to_transmit();
	brt_msn = temptransmit;

	temptransmit = brt_lsn;
	convert_to_transmit();
	brt_lsn = temptransmit;
//;************************************************
if(realtime == 0x30)
	{
   	SBUF = ENC_POT_STATUS;		//;0x28
	transmit_data();
	SBUF = POT_ID;				//;0x58				
	transmit_data();
	SBUF = brt_msn;				//;0-9,A-F	
	transmit_data();
	SBUF = brt_lsn;				//;0-9,A-F	
	transmit_data();
	SBUF = 0x0D;				//;Return	
	transmit_data();
	SBUF = 0x0A;				//;New Line	
	transmit_data();
	}
	old_brt_byte = new_brt_byte;  	
	}
	}
//;************************************************
if(send_pot_status)
    {
   	SBUF = ENC_POT_STATUS;		//;0x28
	transmit_data();
	SBUF = POT_ID;				//;0x58				
	transmit_data();
	SBUF = brt_msn;				//;0-9,A-F	
	transmit_data();
	SBUF = brt_lsn;				//;0-9,A-F	
	transmit_data();
	SBUF = 0x0D;				//;Return	
	transmit_data();
	SBUF = 0x0A;				//;New Line	
	transmit_data();
	send_pot_status = 0;
//;************************************************
	if(echo == 0x31)								//; send encoder echo
	{
	DELAY_LOOP_Wait(100);
   	SBUF = ENC_POT_STATUS;		//;0x28
	transmit_data();
	SBUF = POT_ID;				//;0x58				
	transmit_data();
	SBUF = 0x0D;				//;Return	
	transmit_data();
	SBUF = 0x0A;				//;New Line	
	transmit_data();
	}	
	}
//;************************************************
}
//;************************************************
void update_dimming(void)
{
if(brt_control == 0x31)
{
if(update_display_dimming_bit)
	{
	s_display_dim_msn = display_dim_msn;
	s_display_dim_lsn = display_dim_lsn;

	tempreceive = display_dim_msn;
	convert_to_receive();
	display_dim_msn = tempreceive;
    
	tempreceive = display_dim_lsn;
    convert_to_receive();
	display_dim_lsn = tempreceive; 
   	
	display_dim_msn = (display_dim_msn << 4) & 0xF0;
   	display_dim_lsn = display_dim_lsn & 0x0F;
   	
	display_dim_byte = display_dim_msn | display_dim_lsn;
//;************************************************	
	if(display_dim_byte == 0x00)
	{PBYTE[0x82] = 0x00;}
	else
	{PBYTE[0x82] = 0x01;}
//;************************************************	
	if(display_dim_byte < 0x80)
	{
	day_night = 0;
	PWM0L = dim_value[display_dim_byte];
	PWM0H = ~bezel_dim_byte;		
	}
//;************************************************
if((old_display_dim_byte < 0x80) && (display_dim_byte > 0x7F))
	{   
    PBYTE[0x82] = 0x00;
	PWM0L = dim_value[display_dim_byte];	
	PWM0H = ~bezel_dim_byte;
	DELAY_LOOP_Wait(25);
	day_night = 1;
    DELAY_LOOP_Wait(25);
	PBYTE[0x82] = 0x01;
	}							 
//;************************************************	
	if(display_dim_byte > 0x7F)
	{
	day_night = 1;
	PWM0L = dim_value[display_dim_byte];		
	PWM0H = ~bezel_dim_byte;		
	}
//;************************************************
	old_display_dim_byte = display_dim_byte;
//;************************************************    
	if(echo == 0x31)								//; send display dim echo
	{
  	SBUF = CONTROL_COMMAND;
	transmit_data();
  	SBUF = DISPLAY_DIM_ID;
	transmit_data();
	SBUF = s_display_dim_msn;
	transmit_data();
  	SBUF = s_display_dim_lsn;
	transmit_data();
  	SBUF = 0x0D;
	transmit_data();
  	SBUF = 0x0A;
	transmit_data();
	}   	
//;************************************************	
	update_display_dimming_bit = 0;
//;************************************************
 	}
}
//;************************************************
if(update_bezel_dimming_bit)
 	{
	s_bezel_dim_msn = bezel_dim_msn;
	s_bezel_dim_lsn = bezel_dim_lsn;

	tempreceive = bezel_dim_msn;
	convert_to_receive();
	bezel_dim_msn = tempreceive;
    
	tempreceive = bezel_dim_lsn;
    convert_to_receive();
	bezel_dim_lsn = tempreceive; 
   	
	bezel_dim_msn = (bezel_dim_msn << 4) & 0xF0;
   	bezel_dim_lsn = bezel_dim_lsn & 0x0F;
   	
	bezel_dim_byte = bezel_dim_msn | bezel_dim_lsn;
	
	//PWM0L = dim_value[display_dim_byte];
	//PWM0H = ~bezel_dim_byte;
	
	if(brt_control == 0x30)
	{
	PWM0L = dim_value[old_brt_byte];
	//display_dim_byte = dim_value[brt_byte];
	PWM0H = ~bezel_dim_byte;
	}
	else
	{
	PWM0L = dim_value[display_dim_byte];
	//brt_byte = dim_value[display_dim_byte];
	PWM0H = ~bezel_dim_byte;
	}
//;************************************************    
	if(echo == 0x31)								//; send bezel dim echo
	{
  	SBUF = CONTROL_COMMAND;
	transmit_data();
  	SBUF = BEZEL_DIM_ID;
	transmit_data();
	SBUF = s_bezel_dim_msn;
	transmit_data();
  	SBUF = s_bezel_dim_lsn;
	transmit_data();
  	SBUF = 0x0D;
	transmit_data();
  	SBUF = 0x0A;
	transmit_data();
	}
//;************************************************    	
	update_bezel_dimming_bit = 0;
//;************************************************ 
 	}  
}
//;************************************************
void send_status(void)
{
if(send_status_bit)
	{
//;************************************************
	ACC	= PANEL_STATUS;
	SBUF = ACC;							//1
	transmit_data();
//;************************************************
	ACC = PB_41_Status;					//2 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_42_Status;					//3
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_43_Status;					//4 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_44_Status;					//5
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_45_Status;					//6
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_46_Status;					//7
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_47_Status;					//8 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_48_Status;					//9
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_49_Status;					//10
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_4A_Status;					//11
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_4B_Status;					//12
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_4C_Status;					//13 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_4D_Status;					//14	
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_4E_Status;					//15 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_4F_Status;					//16 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_50_Status;					//17
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = PB_51_Status;					//18 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = BRT_PB_Status;				//19 
	SBUF = ACC;	
	transmit_data();	
//;************************************************
	ACC = ENC_PB_Status;				//20 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = 0x00;							//21
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = 0x00;							//22 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = brt_msn;						//23 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = brt_lsn;						//24
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = enc_msn;						//25
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = enc_lsn;						//26
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = s_display_dim_msn;			//27 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = s_display_dim_lsn;			//28 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = s_contrast_msn;				//29
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = s_contrast_lsn;				//30
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = s_bezel_dim_msn;				//31
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = s_bezel_dim_lsn;				//32
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = s_skidball_msn;				//33 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = s_skidball_lsn;				//34
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = 0x00;							//35
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = 0x00;							//36
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = 0x0D;							//37 
	SBUF = ACC;	
	transmit_data();
//;************************************************
	ACC = 0x0A;							//38
	SBUF = ACC;	
	transmit_data();
//;************************************************
	if(echo == 0x31)								//; send panel status request echo
	{
	DELAY_LOOP_Wait(100);
  	SBUF = PANEL_STATUS;
	transmit_data();
  	SBUF = 0x0D;
	transmit_data();
  	SBUF = 0x0A;
	transmit_data();
	}
	send_status_bit = 0;
//;************************************************
	}
}
//;************************************************
void send_single_sw_status(void)
{
if(send_single_switch_status)
	{
//;************************************************
	if(switch_id == 0x41)
	{
	Switch_Data = PB_41_Status;
	switch_send();
	}	
	if(switch_id == 0x42)
	{
	Switch_Data = PB_42_Status;
	switch_send();
	}	
	if(switch_id == 0x43)
	{
	Switch_Data = PB_43_Status;
	switch_send();
	}
	if(switch_id == 0x44)
	{
	Switch_Data = PB_44_Status;
	switch_send();
	}
	if(switch_id == 0x45)
	{
	Switch_Data = PB_45_Status;
	switch_send();
	}
	if(switch_id == 0x46)
	{
	Switch_Data = PB_46_Status;
	switch_send();
	}
	if(switch_id == 0x47)
	{
	Switch_Data = PB_47_Status;
	switch_send();
	}
	if(switch_id == 0x48)
	{
	Switch_Data = PB_48_Status;
	switch_send();
	}
	if(switch_id == 0x49)
	{
	Switch_Data = PB_49_Status;
	switch_send();
	}
	if(switch_id == 0x4A)
	{
	Switch_Data = PB_4A_Status;
	switch_send();
	}
	if(switch_id == 0x4B)
	{
	Switch_Data = PB_4B_Status;
	switch_send();
	}
	if(switch_id == 0x4C)
	{
	Switch_Data = PB_4C_Status;
	switch_send();
	}
	if(switch_id == 0x4D)
	{
	Switch_Data = PB_4D_Status;
	switch_send();
	}
	if(switch_id == 0x4E)
	{
	Switch_Data = PB_4E_Status;
	switch_send();	   
	}
	if(switch_id == 0x4F)
	{
	Switch_Data = PB_4F_Status;
	switch_send();
	}
	if(switch_id == 0x50)
	{
	Switch_Data = PB_50_Status;
	switch_send();
	}
	if(switch_id == 0x51)
	{
	Switch_Data = PB_51_Status;
	switch_send();
	}
	if(switch_id == 0x52)
	{
	Switch_Data = BRT_PB_Status;
	switch_send();
	}
	if(switch_id == 0x53)
	{
	Switch_Data = ENC_PB_Status;
	switch_send();
	}
//;************************************************    
	if(echo == 0x31)								//; send echo
	{
	DELAY_LOOP_Wait(100);
  	SBUF = SWITCH_STATUS;
	transmit_data();
	SBUF = switch_id;
	transmit_data();
  	SBUF = 0x0D;
	transmit_data();
  	SBUF = 0x0A;
	transmit_data();
	}
	send_single_switch_status = 0;
//;************************************************
	}
}
//;************************************************
void switch_send(void)
{
  	SBUF = SWITCH_STATUS;
	transmit_data();
  	SBUF = switch_id;
	transmit_data();
  	SBUF = Switch_Data;
	transmit_data();
  	SBUF = 0x0D;
	transmit_data();
  	SBUF = 0x0A;
	transmit_data();	
}
//;************************************************
void send_command_values(void)
{
if(send_display_dim_values)
	{
	SBUF = CONTROL_COMMAND;
	transmit_data();
//;************************************************  	
	SBUF = DISPLAY_DIM_ID;
	transmit_data();
//;************************************************  	
	SBUF = s_display_dim_msn;
	transmit_data();
//;************************************************
	SBUF = s_display_dim_lsn;
	transmit_data();
//;************************************************  	
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
//;************************************************	
	send_display_dim_values = 0;
//;************************************************
	if(echo == 0x31)								//; send command dim echo
	{
	DELAY_LOOP_Wait(100);
	SBUF = CONTROL_COMMAND;
	transmit_data();
//;************************************************  	
	SBUF = DISPLAY_DIM_ID;
	transmit_data();
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
	}	
	}
//;************************************************
if(send_contrast_values)
	{
	SBUF = CONTROL_COMMAND;
	transmit_data();
//;************************************************  	
	SBUF = CONTRAST_ID;
	transmit_data();
//;************************************************  	
	SBUF = s_contrast_msn;
	transmit_data();
//;************************************************
	SBUF = s_contrast_lsn;
	transmit_data();
//;************************************************  	
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
//;************************************************	
	send_contrast_values = 0;
//;************************************************
	if(echo == 0x31)								//; send command dim echo
	{
	DELAY_LOOP_Wait(100);
	SBUF = CONTROL_COMMAND;
	transmit_data();
//;************************************************  	
	SBUF = CONTRAST_ID;
	transmit_data();
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
	}	
	}
//;************************************************
if(send_bezel_dim_values)
	{
	SBUF = CONTROL_COMMAND;
	transmit_data();
//;************************************************  	
	SBUF = BEZEL_DIM_ID;
	transmit_data();
//;************************************************  	
	SBUF = s_bezel_dim_msn;
	transmit_data();
//;************************************************
	SBUF = s_bezel_dim_lsn;
	transmit_data();
//;************************************************  	
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
//;************************************************
	send_bezel_dim_values = 0;
//;************************************************	
	if(echo == 0x31)								//; send command dim echo
	{
	DELAY_LOOP_Wait(100);
	SBUF = CONTROL_COMMAND;
	transmit_data();
//;************************************************  	
	SBUF = BEZEL_DIM_ID;
	transmit_data();
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
	}
//;************************************************	
	}
//;************************************************
if(send_skidball_values)
	{
	SBUF = CONTROL_COMMAND;
	transmit_data();
//;************************************************  	
	SBUF = SKIDBALL_ID;
	transmit_data();
//;************************************************  	
	SBUF = s_skidball_msn;
	transmit_data();
//;************************************************
	SBUF = s_skidball_lsn;
	transmit_data();
//;************************************************  	
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
//;************************************************	
	send_skidball_values = 0;
//;************************************************
	if(echo == 0x31)								//; send command dim echo
	{
	DELAY_LOOP_Wait(100);
	SBUF = SKIDBALL_ID;
	transmit_data();
//;************************************************  	
	SBUF = BEZEL_DIM_ID;
	transmit_data();
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
	}
	}
//;************************************************
}
//;************************************************
void send_option_values(void)
{
if(send_echo_value)
	{
	ACC = OPTIONS;
	SBUF = ACC;
	transmit_data();
//;************************************************  	
	ACC = ECHO_OPTION_ID;
	SBUF = ACC;
	transmit_data();
//;************************************************  	
	ACC = 0x30;
	SBUF = ACC;
	transmit_data();
//;************************************************
	ACC = echo;
	SBUF = ACC;
	transmit_data();
//;************************************************  	
	ACC = 0x0D;
	SBUF = ACC;
	transmit_data();
//;************************************************  	
	ACC = 0x0A;
	SBUF = ACC;
	transmit_data();
//;************************************************	
	send_echo_value = 0;
//;************************************************
	if(echo == 0x31)								//; send command dim echo
	{
	DELAY_LOOP_Wait(100);
	SBUF = OPTIONS;
	transmit_data();
//;************************************************  	
	SBUF = ECHO_OPTION_ID;
	transmit_data();
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
	}	
	}
//;************************************************
if(send_realtime_value)
	{
	SBUF = OPTIONS;
	transmit_data();
//;************************************************  	
	SBUF = REALTIME_OPTION_ID;
	transmit_data();
//;************************************************  	
	SBUF = 0x30;
	transmit_data();
//;************************************************
	SBUF = realtime;
	transmit_data();
//;************************************************  	
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
//;************************************************	
	send_realtime_value = 0;
//;************************************************
	if(echo == 0x31)								//; send command dim echo
	{
	DELAY_LOOP_Wait(100);
	SBUF = OPTIONS;
	transmit_data();
//;************************************************  	
	SBUF = REALTIME_OPTION_ID;
	transmit_data();
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
	}
	}
//;************************************************
if(send_lcd_value)
	{
	SBUF = OPTIONS;
	transmit_data();
//;************************************************  	
	SBUF = BRT_CTRL_OPTION_ID;
	transmit_data();
//;************************************************  	
	SBUF = 0x30;
	transmit_data();
//;************************************************
	SBUF = brt_control;
	transmit_data();
//;************************************************  	
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
//;************************************************	
	send_lcd_value = 0;
//;************************************************
	if(echo == 0x31)								//; send command dim echo
	{
	DELAY_LOOP_Wait(100);
	SBUF = OPTIONS;
	transmit_data();
//;************************************************  	
	SBUF = BRT_CTRL_OPTION_ID;
	transmit_data();
	SBUF = 0x0D;
	transmit_data();
//;************************************************  	
	SBUF = 0x0A;
	transmit_data();
	}
	}
//;************************************************
}
//;************************************************
void SER_INT(void) interrupt 4
{
unsigned char hold_byte,temp_byte,control_byte,option_byte;

if(!TI)
TI = 0;
//;************************************************
	if (RI)
	{hold_byte = SBUF;						
	 RI = 0;
//;************************************************									
		switch(hold_byte)						// 
			 {
//;************************************************									
			case SWITCH_STATUS:					//; 0x24
				receive_data();	
				switch_id = SBUF;
				receive_data();					//; 0x0D
				receive_data();					//; 0x0A
				send_single_switch_status = 1;
			break;
//;************************************************
			case PANEL_STATUS:					//; 0x26
				receive_data();					//; 0x0D
				receive_data();					//; 0x0A
				send_status_bit = 1;			
			break;
//;************************************************
			case CONTROL_COMMAND:				//; 0x23				 
				 
				receive_data();
				temp_byte = SBUF;
//;************************************************
//;DISPLAY DIMMING
//;************************************************			
			if(temp_byte == DISPLAY_DIM_ID)
				{      			
				receive_data();
				control_byte = SBUF;
				if(control_byte == 0x0D)
				{
				receive_data();					//; 0x0A
				send_display_dim_values = 1;				
				break;
				}
				else
				{
				display_dim_msn = SBUF;
				receive_data();
				display_dim_lsn = SBUF;
				update_display_dimming_bit = 1;
				}
				}
//;************************************************
//;DISPLAY CONTRAST	
//;************************************************		
			if(temp_byte == CONTRAST_ID)
				{      			
				receive_data();
				control_byte = SBUF;
				if(control_byte == 0x0D)
				{				
				receive_data();					//; 0x0A
				send_contrast_values = 1;
				break;
				}
				else
				{
				contrast_msn = SBUF;
				receive_data();
				contrast_lsn = SBUF;
				update_display_contrast_bit = 1;
				}
				}
//;************************************************			
//:BEZEL DIMMING
//;************************************************			
			if(temp_byte == BEZEL_DIM_ID)
				{
				receive_data();
				control_byte = SBUF;
				if(control_byte == 0x0D)
				{
				receive_data();					//; 0x0A
				send_bezel_dim_values = 1;
				break;
				}
				else
				{
				bezel_dim_msn = SBUF;				
				receive_data();
				bezel_dim_lsn = SBUF;
				update_bezel_dimming_bit = 1;
				}
				}
//;************************************************								
//;SKIDBALL CONTROL
//;************************************************
			if(temp_byte == SKIDBALL_ID)
				{
				receive_data();
				control_byte = SBUF;
				if(control_byte == 0x0D)
				{
				receive_data();					//; 0x0A
				send_skidball_values = 1;
				break;
				}
				else
				{
				skidball_msn = SBUF;				
				receive_data();
				skidball_lsn = SBUF;
				update_skidball_bit = 1;
				}
				}
				receive_data();					//; 0x0D
				receive_data();					//; 0x0A
			break;							
//;************************************************
			case OPTIONS:						//; 0x25
				receive_data();
				temp_byte = SBUF;
//;************************************************
//;ECHO OPTION
//;************************************************			
			if(temp_byte == ECHO_OPTION_ID)
				{
				receive_data();
				option_byte = SBUF;
				if(option_byte == 0x0D)
				{
				receive_data();					//; 0x0A
				send_echo_value = 1;
				break;
				}
				else
				{												
				receive_data();
				echo = SBUF;
				}
				}
//;************************************************
//;SEND ON CHANGE OPTION
//;************************************************			
			if(temp_byte == REALTIME_OPTION_ID)
				{
				receive_data();
				option_byte = SBUF;
				if(option_byte == 0x0D)
				{
				receive_data();					//; 0x0A
				send_realtime_value = 1;
				break;
				}
				else
				{
				receive_data();
				realtime = SBUF;
				}
				}
//;************************************************			
//:LCD BRIGHTNESS CONTROL OPTION
//;************************************************			
			if(temp_byte == BRT_CTRL_OPTION_ID)
				{
				receive_data();
				option_byte = SBUF;
				if(option_byte == 0x0D)
				{
				receive_data();					//; 0x0A
				send_lcd_value = 1;
				break;
				}
				else
				{
				receive_data();
				brt_control = SBUF;
				}
				}				
				receive_data();					//; 0x0D
				receive_data();					//; 0x0A
			break;
//;************************************************
			case ENC_POT_STATUS:				//; 0x28	

				receive_data();
				temp_byte = SBUF;
//;BRT POT STATUS			
			if(temp_byte == POT_ID)
				{
				send_pot_status = 1;
				} 
//;ENC STATUS			
			if(temp_byte == ENC_ID)
				{
				send_encoder_status = 1;
				}								
				receive_data();					//; 0x0D 				
				receive_data();					//;	0x0A
			break;
//;************************************************					
			}									//; end case
//;************************************************		
	}											//end if RI
}
//;************************************************
//void send_programs(void)
//{
//unsigned char i;
//if(send_programs_bit)					// check to see if send program flag is set
//	{
//   		SBUF = FIRMWARE_COMMAND;	
//		transmit_data();
//;micro program***********************************			
//	for(i = 0; i < 4; i++)
//		{	
//		SBUF = RS_micro_firmware[i];	
//		transmit_data();				    
//		}
//;fpga program************************************			
//	for(i = 0; i < 4; i++)
//		{			
//		SBUF = RS_fpga_firmware[i];
//		transmit_data();	
//		}
//;************************************************				
//	send_programs_bit = 0;				// clear the flag
//	}
//
//;************************************************