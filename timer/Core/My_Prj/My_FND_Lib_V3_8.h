/*
 my_Arduino_fnd.h
 *
 *  1907, 8, 22
 *     송명규
      010-2402-4398
     mgsong@hanmail.net
  
  V1.0 ==  아주아주 오래전에.... ??
  V1.1 ==  2022, 8, 27
  V1.2 ==  2022, 10, 12 = 정리, 조건식 추가
  V1.6 ==  2023, 06, 23 = 한국직업능력교육원 에서 추가
  V2.0 ==  2023, 08, 18 = 대폭수정 및 보강 = enum 방식 추가
  V3.0 ==  2023, 09, 04 = R-Pi, MCU, Arduino용으로 통합수정 함 
  V3.2 ==  2023, 09, 13 = Cortex ARM, R-Pi, MCU, Arduino용으로 통합수정 함
  V3.5 ==  2023, 09, 20 = HW Font Data 출력 추가
  V3.6 ==  2024, 01, 16 = ARM 용으로 대폭 보강, DP Macro 제어 추가
  V3.7 ==  2024, 01, 18 = ARM 74HC595 함수 대폭 보강
  V3.8 ==  2025, 03, 25 = DP 처리 보강 == AIOT 2기생
  V3.9 ==  2025, 06, 12 = LED Scan 출력 추가, A-COM 통합, K-COM 통합
*/


#ifndef __My_FND_LIB__  //{  // endif-1
#define __My_FND_LIB__

// 2023, 9, 13 추가
#if ARM_Mcu_Mode
  // 2025, 03, 27추가 = AIOT 2기
  #if F446RE
    #include "stm32f4xx_hal.h"
  #elif F103RB
    #include "stm32f1xx_hal.h"  // F103, F시리즈
  #elif L152RE
    #include "stm32l1xx_hal.h"    //L152/151/162 시리즈
  #endif

  #include "main.h"
  #include <My_ARM_GPIO_Lib_V3_8.h>

  #define FND_Data_Port      GPIOC
  #define FND_en_Port   GPIOA

// 2025, 06, 12 추가
#if Led_Scan
  #define LED_Data_Port    GPIOC
  #define LED_en_Port      GPIOA
  #define Led_scan_pin  led_scan_Pin  //GPIO_PIN_0
#endif

  #define Out_Byte_Low  1 // Bit 0-7
  // #define Out_Byte_Hi   2 // Bit 8 -15

  #define SW_Font       3
  // #define HW_Font       4 // IC Chip 74HC47

#elif (ESP32_Mode || R_PI_Mode || Aduino_Mode || Mcu_8Bit_Mode)  // 2024, 01, 16 추가
  #define FND_Port     PORTB
  #define FND_en_Port  PORTA
#endif

// fnd font data
// K-공통
const unsigned char fnd_font[] =
{
   // 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   H,   L,   E,   o,   P,  F,
   0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f,0x76,0x38,0x79,0x5c,0x73,0x71,
   // C,    d,    A,    u,    T,    r,   b,  blk ,  -
   0x39, 0x5e, 0x77, 0x1c, 0x44, 0x50, 0x7c, 0x00, 0x40
};

// fnd maseg display
#define O   0x0d   //o display
#define F   0x0f    // F display
#define H   0x0a    // H   "
#define L   0x0b    // L   "
#define E   0x0c    // E   "
#define P   0x0e    // P   "
#define C   0x10    // C   "
#define D   0x11    // d   "
#define A   0x12    // A   "
#define U   0x13    // u   "
#define T   0x14    // t   "
#define R   0x15    // r   "
#define B   0x16    // b   "
//#define -   0x18    // -
#define BLK 0x00   // fnd blk display

enum en_font{
     e_font_O = 0x0d,  // O display
     e_font_F = 0x0f,  // F display
     e_font_H = 0x0a,  // H display
     e_font_L = 0x0b,  // L display
     e_font_E = 0x0c,  // E display
     e_font_P = 0x0e,  // P display
     e_font_C = 0x10,  // C display
     e_font_D = 0x11,  // D display
     e_font_A = 0x12,  // A display
     e_font_U = 0x13,  // U display
     e_font_T = 0x14,  // T display
     e_font_R = 0x15,  // R display
     e_font_b = 0x16,  // b display
     e_font_BLK = 0x00,  // fnd blk display
}e_font;
//==================================================================

// 2024, 08, 20 IoT 8기 수정
#if (ARM_Mcu_Mode && FND_GPIO_IF)
   //pin_map = fnd en pin set;
   #define X_1000_Pin  x1000_Pin
   #define X_100_Pin   x100_Pin
   #define X_10_Pin    x10_Pin
   #define X_1_Pin     x1_Pin

enum pin_map{ // pin_map = 열거형 이름정의 + 열거형 변수 선언을 동시에 함
  x1000 = X_1000_Pin, // gpio
  x100  = X_100_Pin,
  x10   = X_10_Pin,
  x1    = X_1_Pin
}fnd_en_pin; // pin = 열거형 변수명 = main 에서 열거형 변수 선언 안 해도 됨


#elif (Mcu_8Bit_Mode || Aduino_Mode || ESP32_Mode || R_PI_Mode)
    // pin map setting
    #define X_1000_Pin  14
    #define X_100_Pin   15
    #define X_10_Pin    16
    #define X_1_Pin     17

   // fnd data
   #if SW_Font
      #define FND_A_Pin  fnd_a_Pin
      #define FND_B_Pin  fnd_b_Pin
      #define FND_C_Pin  fnd_c_Pin
      #define FND_D_Pin  fnd_d_Pin
      #define FND_E_Pin  fnd_e_Pin
      #define FND_F_Pin  fnd_f_Pin
      #define FND_G_Pin  fnd_g_Pin
      #define FND_DP_Pin fnd_dp_Pin

      enum sw_fnd_pin_map{
          fnd_a  = FND_A_Pin, //d14  == A0
          fnd_b  = FND_B_Pin,
          fnd_c  = FND_C_Pin,  //d2
          fnd_d  = FND_D_Pin,
          fnd_e  = FND_E_Pin,
          fnd_f  = FND_F_Pin,
          fnd_g  = FND_G_Pin,
          fnd_dp = FND_DP_Pin
         }fnd_pin;

      void fnd_out(unsigned char data)
       {
         d_out(fnd_a, data%2);     // bit 0 = lsb
         d_out(fnd_b, data/2%2);   // bit 1
         d_out(fnd_c, data/4%2);   // bit 2
         d_out(fnd_d, data/8%2);   // bit 3
         d_out(fnd_e, data/16%2);  // bit 4
         d_out(fnd_f, data/32%2);  // bit 5
         d_out(fnd_g, data/64%2);  // bit 6
         d_out(fnd_dp, data/128%2); // bit 7 = msb
       }

   #elif HW_Font
      enum hw_font_pin_map{
        Data_A = 8,
        Data_B = 9,
        Data_C = 10,
        Data_D = 11
      }fnd_pin;
   #endif
#endif


#define DP_ON	0x80	// ON
#define DP_OFF	0x7f	// OFF

// 변수선언 
struct fnd_v {
  char scan;
  char dp_tg_flag;
  int  fnd_loop;
  char dp1000_flag;
  char dp100_flag;
  char dp10_flag;
  char dp1_flag;
  char dp_digit;
  char scan_595;
}fnd_value = {0, 0, 0, 0, 0, 0, 0, 0, 0};
struct fnd_v *fnd = &fnd_value;

enum DP_Dis
{
  // fnd_dis_A
  DP1000_ON = 1,
  DP100_ON = 2,
  DP10_ON = 3,
  DP1_ON = 4,
  DP_STOP = 0,  // AIOT 2기생 추가

  // fnd_dis_595
  DP_digi_1000_ON = 0x01,
  DP_digi_100_ON = 0x02,
  DP_digi_10_ON = 0x04,
  DP_digi_1_ON = 0x08,
  DP_ALL_OFF = 0x00,
}dp;


#if FND_GPIO_IF
  #if COM_A
   #if en_font_dis //En_font
      void fnd_dis_en_font(char e_font1, char e_font2, char e_font3, char e_font4)
   #elif Led_Scan
      void fnd_led_dis_4(uint16_t d_buf, uint8_t led_buf)
   #elif Fnd_Scan
      void fnd_dis_A(unsigned int d_buf)
   #endif
       {
        #if Fnd_Scan
           fnd_value.scan = (fnd_value.scan > 4) ? 1 : fnd_value.scan + 1;
        #elif Led_Scan
           fnd_value.scan = (fnd_value.scan > 5) ? 1 : fnd_value.scan + 1;
        #endif

         switch(fnd_value.scan)
          {
           case 1: // x1000
                  #if (Aduino_Mode || R_PI_Mode)
        	           d_out(x1, OFF);
                     #if SW_Font
                       fnd_out(~fnd_font[d_buf/1000]); // data out
                     #elif HW_Font
                       FND_Port = d_buf/1000;
                     #endif
                       d_out(x1000, ON);
                  #elif Mcu_8Bit_Mode
                       d_out(x1, OFF);
                       FND_Port = ~fnd_font[d_buf/1000];
                       d_out(x1000, ON);
                  #elif ARM_Mcu_Mode
                     #if Led_Scan
                        // Led ds = Led OFF
                       Out_Bit(LED_en_Port, Led_scan_pin, OFF);  // LED OFF
                     #elif Fnd_Scan
                       // FND x1자리 Off
                       fnd_en_pin = x1;
                       #if COM_A
                         Out_Bit(FND_en_Port, fnd_en_pin, OFF);
                       #else //COM_K
                         Out_Bit(FND_en_Port, fnd_en_pin, _OFF);
                       #endif
                     #endif
                       // FND x1000자리 data Out
                       #if Out_Byte_Hi
                          #if SW_Font
                             if(fnd -> dp1000_flag ==  DP1000_ON)
                               Byte_out_H(FND_Data_Port, ~(fnd_font[d_buf/1000] | DP_ON));
                             else
                               Byte_out_H(FND_Data_Port, ~(fnd_font[d_buf/1000] & DP_OFF));
                          #elif HW_Font
                             Byte_out_H(FND_Data_Port, ~d_buf/1000);
                          #endif
                       #elif Out_Byte_Low
                          #if SW_Font
                            // AIOT2기생 보강
							#if COM_A
							  #if en_font_dis
                                if(fnd -> dp1000_flag == DP1000_ON) Byte_out_L(FND_Data_Port, ~(fnd_font[e_font1] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[e_font1] & DP_OFF));
                              #else
                                if(fnd -> dp1000_flag == DP1000_ON) Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf/1000] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf/1000] & DP_OFF));
							  #endif
                            #else // COM_K
                              #if en_font_dis
                                if(fnd -> dp1000_flag == DP1000_ON) Byte_out_L(FND_Data_Port, (fnd_font[d_buf] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf] & DP_OFF));
                              #else
                                if(fnd -> dp1000_flag == DP1000_ON) Byte_out_L(FND_Data_Port, (fnd_font[d_buf/1000] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf/1000] & DP_OFF));
							  #endif
                            #endif
                          #elif HW_Font
                             Byte_out_L(FND_Data_Port, ~d_buf/1000);
                          #endif
                       #endif
                       // FND x1000 자리 En
                       fnd_en_pin = x1000;
                     #if COM_A
                       Out_Bit(FND_en_Port, fnd_en_pin, ON);
                     #else // COM_K
                       Out_Bit(FND_en_Port, fnd_en_pin, _ON);
                     #endif
                  #endif
             break;

           case 2:  //x100
                  #if (Aduino_Mode || R_PI_Mode)
        	           d_out(x1000, OFF);
                     #if SW_Font                     
        	             fnd_out(~fnd_font[d_buf%1000/100]); // data out
                     #elif HW_Font
                       FND_Port = d_buf%1000/100;
                     #endif
        	           d_out(x100, ON);
                  #elif Mcu_8Bit_Mode
        	           d_out(x1000, OFF);
        	           FND_Port = ~fnd_font[d_buf%1000/100];
                       d_out(x100, ON);
                  #elif ARM_Mcu_Mode
                       fnd_en_pin = x1000;
                       Out_Bit(FND_en_Port, fnd_en_pin, OFF);
                       #if Out_Byte_Hi
                          #if SW_Font
                             if(fnd -> dp100_flag ==  DP100_ON)
                               Byte_out_H(FND_Data_Port, ~(fnd_font[d_buf%1000/100] | DP_ON));
							 else
                               Byte_out_H(FND_Data_Port, ~(fnd_font[d_buf%1000/100] & DP_OFF));
                          #elif HW_Font
                             Byte_out_H(FND_Data_Port, ~d_buf%1000/100);
                          #endif
                      #elif Out_Byte_Low
                          #if SW_Font
                             // AIOT2기생 보강
                             #if en_font_dis
                                if(fnd -> dp100_flag == DP100_ON) Byte_out_L(FND_Data_Port, ~(fnd_font[e_font2] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[e_font2] & DP_OFF));
                             #else
                                if(fnd -> dp100_flag == DP100_ON) Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf%1000/100] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf%1000/100] & DP_OFF));
                             #endif
                          #elif HW_Font
                             Byte_out_L(FND_Data_Port, ~d_buf%1000/100);
                          #endif
                      #endif
                       fnd_en_pin = x100;
                       Out_Bit(FND_en_Port, fnd_en_pin, ON);
                  #endif
             break;
      
           case 3:  // x10
                  #if (Aduino_Mode || R_PI_Mode)
        	           d_out(x100, OFF);
                     #if SW_Font                     
                       fnd_out(~fnd_font[d_buf%100/10]); // data out
                     #elif HW_Font
                       FND_Port = d_buf%100/10;
                     #endif
                       d_out(x10, ON);
                  #elif Mcu_8Bit_Mode
                       d_out(x100, OFF);
                       FND_Port = ~fnd_font[d_buf%100/10];
                       d_out(x10, ON);
                  #elif ARM_Mcu_Mode
                       fnd_en_pin = x100;
                       Out_Bit(FND_en_Port, fnd_en_pin, OFF);
                       #if Out_Byte_Hi
                          #if SW_Font
                             if(fnd -> dp10_flag ==  DP10_ON)
                               Byte_out_H(FND_Data_Port, ~(fnd_font[d_buf%100/10] | DP_ON));
							 else
                               Byte_out_H(FND_Data_Port, ~(fnd_font[d_buf%100/10] & DP_OFF));
                          #elif HW_Font
                             Byte_out_H(FND_Data_Port, ~d_buf%100/10);
                          #endif
                      #elif Out_Byte_Low
                          #if SW_Font
                             // AIOT2기생 보강
                             #if en_font_dis
                                if(fnd -> dp10_flag == DP10_ON) Byte_out_L(FND_Data_Port, ~(fnd_font[e_font3] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[e_font3] & DP_OFF));
                             #else
                                if(fnd -> dp10_flag == DP10_ON) Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf%100/10] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf%100/10] & DP_OFF));
                             #endif
                          #elif HW_Font
                             Byte_out_L(FND_Data_Port, ~d_buf%100/10);
                          #endif
                      #endif
                       fnd_en_pin = x10;
                       Out_Bit(FND_en_Port, fnd_en_pin, ON);
                  #endif
             break;

           case 4: // x1
                  #if (Aduino_Mode || R_PI_Mode)
        	           d_out(x10, OFF);
                     #if SW_Font
                       fnd_out(~fnd_font[d_buf%10]); // data out
                     #elif HW_Font
                       FND_Port = d_buf%10;
                     #endif
                       d_out(x1, ON);
                  #elif Mcu_8Bit_Mode
                       d_out(x10, OFF);
                       FND_Port = ~fnd_font[d_buf%10];
                       d_out(x1, ON);
                  #elif ARM_Mcu_Mode
                       fnd_en_pin = x10;
                       Out_Bit(FND_en_Port, fnd_en_pin, OFF);
                       #if Out_Byte_Hi
                          #if SW_Font
                             if(fnd -> dp1_flag ==  DP1_ON)
                               Byte_out_H(FND_Data_Port, ~(fnd_font[d_buf%10] | DP_ON));
							 else
                               Byte_out_H(FND_Data_Port, ~(fnd_font[d_buf%10] & DP_OFF));
                          #elif HW_Font
                             Byte_out_H(FND_Data_Port, ~d_buf%10);
                          #endif
                       #elif Out_Byte_Low
                          #if SW_Font
                             // AIOT2기생 보강
                             #if en_font_dis
                                if(fnd -> dp1_flag == DP1_ON) Byte_out_L(FND_Data_Port, ~(fnd_font[e_font4] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[e_font4] & DP_OFF));
                             #else
                                if(fnd -> dp1_flag == DP1_ON) Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf%10] | DP_ON));
                                else Byte_out_L(FND_Data_Port, ~(fnd_font[d_buf%10] & DP_OFF));
                             #endif
                          #elif HW_Font
                             Byte_out_L(FND_Data_Port, ~d_buf%10);
                          #endif
                       #endif
                       fnd_en_pin = x1;
                       Out_Bit(FND_en_Port, fnd_en_pin, ON);
                  #endif
             break;

              // LED display = 2025, 06, 12 추가 = AIOT 4기생 추가
           case 5:
              #if Led_Scan
        	    // FND 1자리 OFF
        	    fnd_en_pin = x1;
        	    Out_Bit(FND_en_Port, fnd_en_pin, OFF); // X1 FND Off

        	    // LED Data OUT
                #if Out_Byte_Hi // Bit 8 - 15까지 해당

                #elif Out_Byte_Low // Bit 0 - 7까지 해당
                   // LED Data Out
        	       Byte_out_L(LED_Data_Port, led_buf);
                #endif

        	    // LED EN
        	    Out_Bit(LED_en_Port, Led_scan_pin, ON); // LED ON
              #endif
        	break;
          } //switch-end
       } // func-end

 #elif COM_K   //2023, 09, 13 추가
	#define DP_ON_K		0x7f	// ON
	#define DP_OFF_K	0x80	// OFF

    void fnd_dis_K(unsigned int d_buf)
     {
       fnd_value.scan = (fnd_value.scan > 4) ? 1 : fnd_value.scan + 1;

       switch(fnd_value.scan)
        {
          case 1: // x1000
                  #if (Aduino_Mode || R_PI_Mode)
        	           d_out(x1, _OFF);
                       fnd_out(fnd_font[d_buf/1000]); // data out
                       d_out(x1000, _ON);
                  #elif Mcu_8Bit_Mode
                       d_out(x1, _OFF);
                       FND_Port = fnd_font[d_buf/1000];
                       d_out(x1000, _ON);
                  #elif ARM_Mcu_Mode
                       fnd_en_pin = x1;
                       Out_Bit(FND_en_Port, fnd_en_pin, _OFF);
                       #if Out_Byte_Hi
                           Byte_out_H(FND_Port, fnd_font[d_buf/1000]);
                       #elif Out_Byte_Low
                           Byte_out_L(FND_Port, fnd_font[d_buf/1000]);
                       #endif
                           fnd_en_pin = x1000;
                       Out_Bit(FND_en_Port, fnd_en_pin, _ON);
                  #endif
             break;

          case 2:  //x100
                  #if (Aduino_Mode || R_PI_Mode)
        	           d_out(x1000, _OFF);
        	           fnd_out(fnd_font[d_buf%1000/100]); // data out
        	           d_out(x100, _ON);
                  #elif Mcu_8Bit_Mode
        	           d_out(x1000, _OFF);
        	           FND_Port = fnd_font[d_buf%1000/100];
                       d_out(x100, _ON);
                  #elif ARM_Mcu_Mode
                       fnd_en_pin = x1000;
                       Out_Bit(FND_en_Port, fnd_en_pin, _OFF);
                       #if Out_Byte_Hi
                          Byte_out_H(FND_Port, fnd_font[d_buf%1000/100]);
                      #elif Out_Byte_Low
                          Byte_out_L(FND_Port, fnd_font[d_buf%1000/100]);
                      #endif
                       fnd_en_pin = x100;
                       Out_Bit(FND_en_Port, fnd_en_pin, _ON);
                  #endif
             break;

          case 3:  // x10
                  #if (Aduino_Mode || R_PI_Mode)
        	           d_out(x100, _OFF);
                       fnd_out(fnd_font[d_buf%100/10]); // data out
                       d_out(x10, _ON);
                  #elif Mcu_8Bit_Mode
                       d_out(x100, _OFF);
                       FND_Port = fnd_font[d_buf%100/10];
                       d_out(x10, _ON);
                  #elif ARM_Mcu_Mode
                       fnd_en_pin = x100;
                       Out_Bit(FND_en_Port, fnd_en_pin, _OFF);
                       #if Out_Byte_Hi
                          Byte_out_H(FND_Port, fnd_font[d_buf%100/10]);
                      #elif Out_Byte_Low
                          Byte_out_L(FND_Port, fnd_font[d_buf%100/10]);
                      #endif
                       fnd_en_pin = x10;
                       Out_Bit(FND_en_Port, fnd_en_pin, _ON);
                  #endif
             break;

          case 4: // x1
                  #if (Aduino_Mode || R_PI_Mode)
        	           d_out(x10, _OFF);
                       fnd_out(fnd_font[d_buf%10]); // data out
                       d_out(x1, _ON);
                  #elif Mcu_8Bit_Mode
                       d_out(x10, _OFF);
                       FND_Port = fnd_font[d_buf%10];
                       d_out(x1, _ON);
                  #elif ARM_Mcu_Mode
                       fnd_en_pin = x10;
                       Out_Bit(FND_en_Port, fnd_en_pin, _OFF);
                       #if Out_Byte_Hi
                          Byte_out_H(FND_Port, fnd_font[d_buf%10]);
                      #elif Out_Byte_Low
                          Byte_out_L(FND_Port, fnd_font[d_buf%10]);
                      #endif
                       fnd_en_pin = x1;
                       Out_Bit(FND_en_Port, fnd_en_pin, _ON);
                  #endif
             break;
       }
    }
  #endif
#endif

#if FND_595_IF
  #if ARM_Mcu_Mode
    #define S_data  data_Pin
    #define S_clk   clk_Pin
    #define L_clk   latch_Pin

    #define s_data_port GPIOA
    #define s_clk_port  GPIOA
    #define L_clk_port  GPIOB

   void fnd_dis_595(char scan_f, unsigned char data)
    {
      char k;

       if(fnd -> dp_digit == scan_f) data &= 0x7f;
       else data |= 0x80;

      HAL_GPIO_WritePin(L_clk_port, L_clk, 0);

     for(k = 0; k <= 7; k++)
      {
       if((data & 0x80) == 0x80) HAL_GPIO_WritePin(s_data_port, S_data, 1);
       else HAL_GPIO_WritePin(s_data_port, S_data, 0);

       HAL_GPIO_WritePin(s_clk_port, S_clk, 1);
       //HAL_Delay(1);
       HAL_GPIO_WritePin(s_clk_port, S_clk, 0);

       data <<= 1;
      }

     for(k=0; k<=7; k++)
      {
       if((scan_f & 0x80) == 0x80) HAL_GPIO_WritePin(s_data_port, S_data, 1);
       else HAL_GPIO_WritePin(s_data_port, S_data, 0);

       HAL_GPIO_WritePin(s_clk_port, S_clk, 1);
       //HAL_Delay(1);
       HAL_GPIO_WritePin(s_clk_port, S_clk, 0);

       scan_f <<= 1;
      }

     HAL_GPIO_WritePin(L_clk_port, L_clk, 1);
    }

#if En_Font
   void fnd_595_out_en(uint8_t e_font1, uint8_t e_font2, uint8_t e_font3, uint8_t e_font4)
#else
   void fnd_595_out(uint16_t data)
#endif
    {
	 fnd -> scan_595 = (fnd -> scan_595 > 4) ? 1 : fnd -> scan_595 + 1;

#if En_Font
     if(fnd -> scan_595 == 1) fnd_dis_595(0x01, ~ fnd_font[e_font1]);
     else if(fnd -> scan_595 == 2) fnd_dis_595(0x02, ~fnd_font[e_font2]);
     else if(fnd -> scan_595 == 3) fnd_dis_595(0x04, ~fnd_font[e_font3]);   // x10
     else if(fnd -> scan_595 == 4) fnd_dis_595(0x08, ~fnd_font[e_font4]);     // x1
#else
     if(fnd -> scan_595 == 1) fnd_dis_595(0x01, ~ fnd_font[data/1000]);
     else if(fnd -> scan_595 == 2) fnd_dis_595(0x02, ~fnd_font[data%1000/100]);
     else if(fnd -> scan_595 == 3) fnd_dis_595(0x04, ~fnd_font[data%100/10]);   // x10
     else if(fnd -> scan_595 == 4) fnd_dis_595(0x08, ~fnd_font[data%10]);     // x1
#endif
}

  #elif (Mcu_8Bit_Mode || Aduino_Mode || ESP32_Mode || R_PI_Mode)
     // Arduino, ESP32, R-Pi용


  #endif
#endif

#endif
