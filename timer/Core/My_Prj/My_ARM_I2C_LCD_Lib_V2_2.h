/*
 * My_ARM_I2C_LCD_V2_1.h
 *
 *       송 명 규
 *    010-24-24398
 *  mgsong@hanmail.net
 *
 * v1.0 == 2022, 12, 30 = 최초작성
 * v1.1 == 2023, 09, 12 = 추가
 * v1.2 == 2024, 01, 22 = h, c 통합
 * v2.0 == 2024, 01, 25 = 기능 대폭 보강 및 추가
 * v2.1 == 2025, 01, 17 = 함수명 및 선언 정의명 수정 = AIOT 1기
 * V2.2 == 2025, 03, 27 = LCD test Func 추가 = AIOT 2기
 *
     // 참고로 I2C 칩 어드레스
     // PCF8574   = 0x4E
     // PCF8574AT = 0x3F
     // PCF8574T  = 0x27
 */

#ifndef MY_LIB_MY_ARM_I2C_LCD_LIB_V2_2_H_
#define MY_LIB_MY_ARM_I2C_LCD_LIB_V2_2_H_

// 2025, 03, 27추가 = AIOT 2기
#if F446RE
  #include "stm32f4xx_hal.h"
#elif F103RB
  #include "stm32f1xx_hal.h"  // F103, F시리즈
#elif L152RE
  #include "stm32l1xx_hal.h"    //L152/151/162 시리즈
#endif

#if I2C1_en // 2025, 04, 15 = AIOT 2기생 추가
   extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly
#elif I2C2_en
   extern I2C_HandleTypeDef hi2c2;  // change your handler here accordingly
#elif I2C3_en
   extern I2C_HandleTypeDef hi2c3;  // change your handler here accordingly
#endif

#define SLAVE_ADDRESS_LCD 0x4E   // change this according to ur setup

void lcd_init_i2c(void);   // initialize lcd
void lcd_send_cmd_i2c(char cmd);  // send command to the lcd
void lcd_send_data_i2c(char data);  // send data to the lcd
void lcd_send_string_i2c(char *str);  // send string to the lcd
void lcd_put_cur_i2c(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);
void lcd_clear_i2c(void);

/*
 // 차후 검토 후 수정 예쩡 = 함수 포인터 나 typedef로 수정하는거 검토
typedef void (*lcd_send_cmd)(char cmd);
lcd_send_cmd lcd_send_cmd_i2c = lcd_send_cmd;
*/

// lcd Control Command == 2014, 01, 25 추가
enum LCD_COMMAND_i2c
{
  // lcd all clear
  CLR_i2c  = 0x01,  // lcd clr

  // display move = rr / ll
  L_MV_i2c = 0x18,
  R_MV_i2c = 0x1c,

  //curser move == rr / LL
  CUR_R_MV_i2c = 0x14,
  CUR_L_MV_i2c = 0x10,

  // entry move == rr/ll
  ENT_L_MV_i2c = 0x05,  // 2025, 03, 27 수정 == AIOT 2기
  ENT_R_MV_i2c = 0x06, // default == auto rr move
  ENT_STOP_i2c = 0x07,

  //curser
  CUR_HOME_i2c = 0x02,
  CUR_ON_i2c   = 0x0f,
  CUR_OFF_i2c  = 0x0c,   // 0x0d

  //display
  DIS_ON_i2c   = 0x0c,   // display all on, cirser off
  DIS_OFF_i2c  = 0x0b,

  // lcd Line Home
  DIS_1_HOME_i2c = 0x80,
  DIS_2_HOME_i2c = 0xc0,
  DIS_3_HOME_i2c = 0x94,
  DIS_4_HOME_i2c = 0xd4,

  // lcd IF
  LCD_4_BIT_IF_i2c = 0x28,
  LCD_8_BIT_IF_i2c = 0x38
}lcd_cmd_i2c;


void lcd_send_cmd_i2c(char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0

#if I2C1_en // 2025, 04, 15 = AIOT 2기생 추가
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
#elif I2C2_en
	HAL_I2C_Master_Transmit (&hi2c2, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
#elif I2C3_en
	HAL_I2C_Master_Transmit (&hi2c3, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
#endif
}

void lcd_send_data_i2c(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
#if I2C1_en // 2025, 04, 15 = AIOT 2기생 추가
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
#elif I2C2_en
	HAL_I2C_Master_Transmit (&hi2c2, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
#elif I2C3_en
	HAL_I2C_Master_Transmit (&hi2c3, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
#endif
}

void lcd_clear_i2c (void)
{
	lcd_send_cmd_i2c (0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data_i2c(' ');
	}
}

void lcd_put_cur_i2c(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}

// 2025, 03, 27 == AIOT 2기생 추가
void lcd_init_i2c(void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd_i2c (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd_i2c (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd_i2c (0x30);
	HAL_Delay(10);
	lcd_send_cmd_i2c (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd_i2c(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd_i2c(0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd_i2c(0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd_i2c(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd_i2c(0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	lcd_clear_i2c();
}

void lcd_send_string_i2c(char *str)
{
	while (*str) lcd_send_data_i2c(*str++);
}

//===================================================================================
// 2024, 01, 25 추가
void lcd_gotoxy_i2c(char x, char y)
{
  // lcd display ��ǥ �� = x : 0 ����
  if(y == 1) lcd_send_cmd_i2c(x + 0x80); // 1 line
  else if(y == 2) lcd_send_cmd_i2c(x + 0xc0); // 2 line
  else if(y == 3) lcd_send_cmd_i2c(x + 0x94); // 3 line
  else if(y == 4) lcd_send_cmd_i2c(x + 0xd4); // 4 line
}

void lcd_str_i2c(unsigned char *buf)
{
  // user string data display
  while(*buf != '\0')
   {
     lcd_send_data_i2c(*buf);
     buf++;
   }
}

void lcd_tx_i2c(uint8_t *buf, char lens)
{
  do{
     lcd_send_data_i2c(*buf);
     buf++;
     lens--;
    }while(lens != 0);
}

void lcd_gotoxy_str_i2c(char x, char y, unsigned char *buf)
{
 //
 lcd_gotoxy_i2c(x,y);
 lcd_str_i2c(buf);
}

void lcd_gotoxy_tx_i2c(char x, char y, unsigned char *buf, char lens)
{
 //
 lcd_gotoxy_i2c(x,y);
 lcd_tx_i2c(buf, lens);
}


void lcd_gotoxy_send_i2c(char x, char y, unsigned char buf)
{
//
 lcd_gotoxy_i2c(x,y);
 lcd_send_data_i2c(buf);
}

void lcd_goto_x_clr_i2c(char y, char lens)
{
  char k;
  for(k=0; k <= lens; k++)
  {
	lcd_gotoxy_i2c(k, y);
	lcd_send_data_i2c(' ');  // 0x20
  }
}

void lcd_goto_xy_clr_i2c(char x, char y)
{
  lcd_gotoxy_i2c(x, y);
  lcd_send_data_i2c(' ');
}

void i2c_Lcd_Test()
{
 lcd_gotoxy_i2c(5, 1);
 lcd_str_i2c("Song ");
 lcd_send_data_i2c('K');
 lcd_gotoxy_str_i2c(3, 2, "LCD_Test!!!");

 // Display ON / OFF
 HAL_Delay(100);
 lcd_send_cmd_i2c(DIS_OFF_i2c);
 HAL_Delay(100);
 lcd_send_cmd_i2c(DIS_ON_i2c);
 HAL_Delay(100);
 HAL_Delay(100);
 lcd_send_cmd_i2c(DIS_OFF_i2c);
 HAL_Delay(100);
 lcd_send_cmd_i2c(DIS_ON_i2c);
 HAL_Delay(1000);

 // Curser ON
 lcd_send_cmd_i2c(CUR_ON_i2c);
 HAL_Delay(1000);

 // Display RR/ LL Move
 for(int i = 0; i < 7; i++)
  {
   lcd_send_cmd_i2c(L_MV_i2c);
   HAL_Delay(100);
  }
 HAL_Delay(1000);

 for(int i = 0; i < 7; i++)
  {
   lcd_send_cmd_i2c(R_MV_i2c);
   HAL_Delay(100);
  }
 HAL_Delay(1000);

 // Curser On /OFF
 lcd_send_cmd_i2c(CUR_OFF_i2c);
 HAL_Delay(100);
 lcd_send_cmd_i2c(CUR_ON_i2c);
 HAL_Delay(100);

 // LCD Curser RR / LL Move
 for(int i = 0; i < 7; i++)
  {
	lcd_send_cmd_i2c(CUR_L_MV_i2c);
    HAL_Delay(100);
  }
 HAL_Delay(1000);

 for(int i = 0; i < 7; i++)
  {
    lcd_send_cmd_i2c(CUR_R_MV_i2c);
    HAL_Delay(100);
  }
 HAL_Delay(1000);

 // Curser OFF
 lcd_send_cmd_i2c(CUR_OFF_i2c);
 HAL_Delay(1000);

 // Display CLR
 lcd_send_cmd_i2c(CLR_i2c);
 HAL_Delay(1000);

 // LCD MSG Display
 lcd_gotoxy_str_i2c(5, 3, "LCD_Test_OK");
 lcd_gotoxy_str_i2c(2, 2, "I2C_Test_OK");
 HAL_Delay(3000);

 // Display CLR
 lcd_send_cmd_i2c(CLR_i2c);
 HAL_Delay(1000);

 // Curser Home
 lcd_send_cmd_i2c(CUR_ON_i2c);
 lcd_send_cmd_i2c(CUR_HOME_i2c);
}

//==================================================================
// LCD USER FONT Display == 2024, 01, 22 추가

// lcd user font display

//cg ram address = 0x40 - 0x7f (0x80 - 0xff)
//  i = 0 = 0x40 // User start Adderss = Index = 0(read)
//  i = 1 = 0x47                       = Index = 1(read)
//  i = 2 = 0x50
//  i = 3 = 0x58
//  i = 4 = 0x60
//  i = 5 = 0x68
//  i = 6 = 0x70
//  i = 7 = 0x78                       = Index = 7(read)
//
//  5x8(7) = 8자
//  5x10 = 4자

// lcd user font display
#ifdef Char_LCD_User_Font_i2c
// cg ram start address
#define st_adr_1_i2c    0x40  // 0
#define st_adr_2_i2c    0x48  // 1
#define st_adr_3_i2c    0x50
#define st_adr_4_i2c    0x58
#define st_adr_5_i2c    0x60
#define st_adr_6_i2c    0x68
#define st_adr_7_i2c    0x70
#define st_adr_8_i2c    0x78  // 7

// 2025, 03, 27 추가 == AIOT 2기 추가
enum _END_ADR_I2C_
{
  end_adr_1_i2c = 0,
  end_adr_2_i2c,
  end_adr_3_i2c,
  end_adr_4_i2c,
  end_adr_5_i2c,
  end_adr_6_i2c,
  end_adr_7_i2c,
  end_adr_8_i2c
}ea_i2c;

#define Not_Dis_i2c   0  // 반전
#define Dis_i2c       1  // 비반전


char user_font_1_i2c[8]; // user font Arry
char user_font_2_i2c[8][8];  // user font Arry

void font_wr_8_i2c(char dis)
{
  // 8자 User Font Data를 한번에 다 Write 할 경우에만 사용
  int i;

// User Font WR Func
if(dis == 1)
 { // 비반전 = default
   for(i=0; i <= 7; i++)
    {
     lcd_send_cmd_i2c(0x40+i); lcd_send_data_i2c(home_go_font[0][i]); Delay_ms(1);
     lcd_send_cmd_i2c(0x48+i); lcd_send_data_i2c(home_go_font[1][i]); Delay_ms(1);
     lcd_send_cmd_i2c(0x50+i); lcd_send_data_i2c(home_go_font[2][i]); Delay_ms(1);
     lcd_send_cmd_i2c(0x58+i); lcd_send_data_i2c(home_go_font[3][i]); Delay_ms(1);
     lcd_send_cmd_i2c(0x60+i); lcd_send_data_i2c(home_go_font[4][i]); Delay_ms(1);
     lcd_send_cmd_i2c(0x68+i); lcd_send_data_i2c(home_go_font[5][i]); Delay_ms(1);
     lcd_send_cmd_i2c(0x70+i); lcd_send_data_i2c(home_go_font[6][i]); Delay_ms(1);
     lcd_send_cmd_i2c(0x78+i); lcd_send_data_i2c(home_go_font[7][i]); Delay_ms(1);
    }
  }
 else
  {
	// 반전 = 0
	 for(i=0; i <= 7; i++)
	     {
		  lcd_send_cmd_i2c(0x40+i); lcd_send_data_i2c(~home_go_font[0][i]); Delay_ms(1);
		  lcd_send_cmd_i2c(0x48+i); lcd_send_data_i2c(~home_go_font[1][i]); Delay_ms(1);
		  lcd_send_cmd_i2c(0x50+i); lcd_send_data_i2c(~home_go_font[2][i]); Delay_ms(1);
		  lcd_send_cmd_i2c(0x58+i); lcd_send_data_i2c(~home_go_font[3][i]); Delay_ms(1);
		  lcd_send_cmd_i2c(0x60+i); lcd_send_data_i2c(~home_go_font[4][i]); Delay_ms(1);
		  lcd_send_cmd_i2c(0x68+i); lcd_send_data_i2c(~home_go_font[5][i]); Delay_ms(1);
		  lcd_send_cmd_i2c(0x70+i); lcd_send_data_i2c(~home_go_font[6][i]); Delay_ms(1);
		  lcd_send_cmd_i2c(0x78+i); lcd_send_data_i2c(~home_go_font[7][i]); Delay_ms(1);
	     }
  }
}

void user_font_wr2_i2c(char st_adr, char max_adr, char dis)
{
  // User Font Data를 설정한대로 만 쓰는거 = 최대 8개
  char i, j, k = 0;

  for( i = 0; i <= max_adr; i++) // max 7
    {
     lcd_send_cmd_i2c(st_adr + (i<<3));

     for( j = 0; j <= 7; j++)
      {
       if(dis == 0) lcd_send_data_i2c(~user_font_2_i2c[i][j]); // 반전 = 0
       else	lcd_send_data_i2c(user_font_2_i2c[i][j]);          // 비반전 = 1
      }
   }
}

void user_font_wr_8arry_i2c(char st_adr, char max_adr, char dis)
{
	for(int i = 0; i <= max_adr; i++) // max 8
	    {
		  switch(i)
		  {
		    case 0: memcpy(user_font_1_i2c, Heart, sizeof(Heart)); break;
		    case 1: memcpy(user_font_1_i2c,  Bell, sizeof(Bell)); break;
		    case 2: memcpy(user_font_1_i2c, Alien, sizeof(Alien)); break;
		    case 3: memcpy(user_font_1_i2c, Check, sizeof(Check)); break;
		    case 4: memcpy(user_font_1_i2c, Sound, sizeof(Sound)); break;
		    case 5: memcpy(user_font_1_i2c, Skull, sizeof(Skull)); break;
		    case 6: memcpy(user_font_1_i2c, Lock, sizeof(Lock)); break;
		    case 7: memcpy(user_font_1_i2c, Speaker, sizeof(Speaker)); break;
		  }

		  lcd_send_cmd_i2c(st_adr + (i<<3));

	     for(int j = 0; j <= 7; j++)
	      {
	       if(dis == 0) lcd_send_data_i2c(~user_font_1_i2c[j]); // 반전 = 0
	       else	lcd_send_data_i2c(user_font_1_i2c[j]);          // 비반전 = 1
	      }
	   }
}

void user_font_wr1_arry_i2c(char st_adr, char dis)
{
  // 1개의 1차원 배열 User Font을 하나의 어드레스에 쓸경우 사용
  // 이 함수 사용시에는 user_font 에다가 memcpy 함수를 사용해서 덤프링 해야 됨
  char k;
  for(k=0; k< 8; k++)
   {
     lcd_cmd(st_adr + k);
     if(dis == 0) lcd_send(~user_font_1_i2c[k]);
     else lcd_send(user_font_1_i2c[k]);
   }
}

void user_font_wr1_i2c(char st_adr, uint8_t *font_d, char dis)
{
  // 1개의 1차원 배열 User Font을 하나의 어드레스에 쓸경우 사용
  char k;
  for(k=0; k < 8; k++)
   {
     lcd_send_cmd_i2c(st_adr + k);
     if(dis == 0) lcd_send_data_i2c(~font_d[k]); // 반전
     else lcd_send_data_i2c(font_d[k]);
   }
}
//--------------------------------------------------

// User Font Read Func
void user_font_rd8_i2c()
 {
  // 먼저 쓰고 그다음 읽기
   lcd_send_data_i2c(0x00);  // 0x40 = ST_ADR 1
   lcd_send_data_i2c(0x01);  // 0x48 = ST_ADR 2
   lcd_send_data_i2c(0x02);
   lcd_send_data_i2c(0x03);
   lcd_send_data_i2c(0x04);
   lcd_send_data_i2c(0x05);
   lcd_send_data_i2c(0x06);
   lcd_send_data_i2c(0x07); // 0x78 = = ST_ADR 8
}

void user_font_rd1_i2c(char st_adr)
{
 lcd_send_data_i2c(st_adr);
}

void user_font_arry1_dis(char *ptr, char x, char y, char dis)
{
 // 1차원 배열 == 미 검증 == 차후 검증 예정
 memcpy(user_font_1_i2c,  ptr, sizeof(ptr));
 user_font_wr1_i2c(st_adr_1_i2c, 7, dis);
 HAL_Delay(100);
 lcd_gotoxy_i2c(x, y);
 user_font_rd8_i2c();
}

void user_font_arry2_dis(char *ptr, char x, char y, char dis)
{
 // 2원 배열 == 미 검증 == 차후 검증 예정
 memcpy(user_font_2_i2c,  ptr, sizeof(ptr));
 user_font_wr2_i2c(st_adr_1_i2c, 7, dis);
 HAL_Delay(100);
 lcd_gotoxy_i2c(x, y);
 user_font_rd8_i2c();
}
#endif

#endif /* MY_LIB_MY_ARM_I2C_LCD_LIB_V2_2_H_ */
