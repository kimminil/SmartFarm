

// My Gpio lib
// Song Myoung Gyu
// 010-2402-4398
// End Ver = V3.6 = 2025, 04, 03

// Ver1.0 = 2018, 07, 09
// Ver2.0 = 2019, 11, 08 //
// Ver3.0 = 2020, 03, 12
//    - AVR GPIO Bit  = bit_set, bit_clr, bit_tg, bit-chk
//    - AVR GPIO Port = PortA_L, PortA_H, PortB_L, PortB_H, PortC_L, PortC_H,
// Ver3.2 = 2020, 04, 21
//      - gpio write func Macro func 추가
// Ver 3.4 == 2024, 01, 10 = BIt func 추가
// Ver 3.5 == 2024, 01, 16 = Arduino Map 함수 구현 추가
// Ver 3.6 == 2025, 04, 03 = 임의의 포트의 임의의 Bit로 8Bit Port 구성 함수 추가 = AIOT 1개생
// Ver 3.7 == 2025, 06, 10 = Bit 제어 함수 추가 = AIOT 4개생
// Ver 3.8 == 2025, 06, 20 = 임의이 포트의 임의의 Bit로 8Bit 입력 Portr구성 함수 추가


#ifndef __my_gpio_lib__
#define __my_gpio_lib__

// 2025, 06, 10 추가 = AIOT 4기생
#if F446RE
   #include "stm32f4xx_hal.h" // F446 시리즈
   #include "main.h"
#elif F103RB
   #include "stm32f1xx_hal.h"  // F103, F시리즈
   #include "main.h"
#elif L152RE
   #include "stm32l1xx_hal.h"    //L152/151/162 시리즈
   #include "main.h"
#endif


// 소스방식
#define ON    1
#define OFF   0

// 싱크방식
#define _ON   0
#define _OFF  1

#define Low  GPIO_PIN_RESET
#define Hi   GPIO_PIN_SET

// 1. GPIO OUTPUT LIB
#define Out_Bit(port, pin, out_data_bits) HAL_GPIO_WritePin(port, pin, out_data_bits)
#define Out_Bit_Tg(port, pin)  HAL_GPIO_TogglePin(port, pin)

// Ver 3.2 = 2020, 04, 21
#define Out_Bit_A(pin, out_data_bits) HAL_GPIO_WritePin(GPIOA, pin, out_data_bits)
#define Out_Bit_B(pin, out_data_bits) HAL_GPIO_WritePin(GPIOB, pin, out_data_bits)
#define Out_Bit_C(pin, out_data_bits) HAL_GPIO_WritePin(GPIOC, pin, out_data_bits)

//
#define Led_out_A_bit(pin, out_data_bits) Out_Bit_A(pin, (out_data_bits) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define Led_out_B_bit(pin, out_data_bits) Out_Bit_B(pin, (out_data_bits) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define Led_out_C_bit(pin, out_data_bits) Out_Bit_C(pin, (out_data_bits) ? GPIO_PIN_SET : GPIO_PIN_RESET)

//2. GPIO INPUT LIB
#define In_Bit(port, pin) HAL_GPIO_ReadPin(port, pin)

//3. SyStem delay LIB
#define Delay_ms(val)  HAL_Delay(val)

// Arduino Map 함수 구현 == 2024, 01, 16 추가
long my_map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//4. Bit Macro Func == 2024, 01, 10 추가
void bit_tg(GPIO_TypeDef *GPIOx, uint16_t Bit)
{
  // 사용시 주의 필요 = 특히 아래 bit_tg_1 하고 연속하여 같이 사용하지 마세요
  uint32_t buf;

  buf = GPIOx->ODR;
  GPIOx-> ODR = ~(buf & Bit); // GPIO_PIN_5 잘됨
}

// AIOT 4기생 == 2025, 06, 10 추가검증 완료
void bit_tg_1(GPIO_TypeDef *GPIOx, uint16_t Bit)
{
  // 전체 토클
  GPIOx-> ODR ^= (1 << Bit); // 5는 잘됨, GPIO_PIN_5는 안됨
}

void bit_set(GPIO_TypeDef *GPIOx, uint16_t Bit)
{
 Out_Bit(GPIOx, Bit, GPIO_PIN_SET);
}


// AIOT 4기생 == 2025, 06, 10 추가검증 완료
void bit_set_1(GPIO_TypeDef *GPIOx, uint16_t Bit)
{
 GPIOx ->ODR |= (1 << Bit); // org = GPIO_PIN_5 동작안함
}

void bit_clr(GPIO_TypeDef *GPIOx, uint16_t Bit)
{
  Out_Bit(GPIOx, Bit, GPIO_PIN_RESET);
}

// AIOT 4기생 == 2025, 06, 10 추가검증 완료
void bit_clr_1(GPIO_TypeDef *GPIOx, uint16_t Bit)
{
 GPIOx ->ODR &= ~(1 << Bit); //org = GPIO_PIN_5 동작함
}

// AIOT 2기생 == 2025, 03, 24 추가검증 완료
int bit_chk(GPIO_TypeDef *GPIOx, uint16_t Bit)
{
  return (GPIOx -> ODR & (1<< Bit)); //GPIO_PIN_5 동작 안됨
}

// AIOT 4기생 == 2025, 06, 11 추가검증 완료
void bit_to_byte(GPIO_TypeDef *GPIOx, uint16_t Bit)
{
  uint16_t buf = 0;

  for(int k = 7; k >=0; k--)
   {
	if((Bit & 0x80) == 0x80) buf |= (1 << k);
	else buf &= ~(1 << k);

	Bit <<= 1;
   }

 GPIOx -> ODR = ~buf;
}

// 2025, 04, 01 == AIOT 2기 추가
// 출력 핀 정의 = 핀번호는 PRj에 맞게 수정하여 사용하세요
struct GPIO_OUT
{
  const uint16_t OUT_Bit_0;
  const uint16_t OUT_Bit_1;
  const uint16_t OUT_Bit_2;
  const uint16_t OUT_Bit_3;
  const uint16_t OUT_Bit_4;
  const uint16_t OUT_Bit_5;
  const uint16_t OUT_Bit_6;
  const uint16_t OUT_Bit_7;
}Out = {
//		led1_Pin,
//		led2_Pin,
//		led3_Pin,
//		led4_Pin,
		//led4_Pin,
		//led5_Pin,
		//led6_Pin,
		//led7_Pin   //GPIO_PIN_7
       }; // 핀 번호는 프로젝트에 맞게 수정하여 사용하세요
struct GPIO_OUT *Gpio_Out = &Out;

// 2025, 06, 10 추가 = AIOT4기생 추가
//uint8_t gpio_led[8] = {led0_Pin,led1_Pin,led2_Pin,led3_Pin,led4_Pin,led5_Pin,led6_Pin,led7_Pin};

char GPIO_Pin_Arry[8] = {
						 GPIO_PIN_0,
						 GPIO_PIN_1,
						 GPIO_PIN_2,
						 GPIO_PIN_3,
						 GPIO_PIN_4,
						 GPIO_PIN_5,
						 GPIO_PIN_6,
						 GPIO_PIN_7
                        };

void My_Port_Byte_Wr(unsigned char data)
{
 Out_Bit(GPIOB, Gpio_Out -> OUT_Bit_0, data%2);     // bit 0 = lsb
 Out_Bit(GPIOC, Gpio_Out -> OUT_Bit_1, data/2%2);   // bit 1
 Out_Bit(GPIOC, Gpio_Out -> OUT_Bit_2, data/4%2);   // bit 2
 Out_Bit(GPIOA, Gpio_Out -> OUT_Bit_3, data/8%2);   // bit 3
 Out_Bit(GPIOB, Gpio_Out -> OUT_Bit_4, data/16%2);  // bit 4
 Out_Bit(GPIOB, Gpio_Out -> OUT_Bit_5, data/32%2);  // bit 5
 Out_Bit(GPIOB, Gpio_Out -> OUT_Bit_6, data/64%2);  // bit 6
 Out_Bit(GPIOB, Gpio_Out -> OUT_Bit_7, data/128%2); // bit 7 = msb


/*
	GPIOB -> BSRR |= ((data%2) == 1) ? GPIO_BSRR_BS10  // 1
			                         : GPIO_BSRR_BR10; // 0

	  GPIOC -> BSRR |= ((data/2%2) == 1) ? GPIO_BSRR_BS8  // 1
	 		                             : GPIO_BSRR_BR8;  // 0

	  GPIOC -> BSRR |= ((data/4%2) == 1) ? GPIO_BSRR_BS9  // 1
	  		                             : GPIO_BSRR_BR9;  // 0

	  GPIOA -> BSRR |= ((data/8%2) == 1) ? GPIO_BSRR_BS8  // 1
	  		                             : GPIO_BSRR_BR8;  // 0

	  GPIOB -> BSRR |= ((data/16%2) == 1) ? GPIO_BSRR_BS12  // 1
	  		                              : GPIO_BSRR_BR12;  // 0

	  GPIOB -> BSRR |= ((data/32%2) == 1) ? GPIO_BSRR_BS13  // 1
	   		                              : GPIO_BSRR_BR13;  // 0

	  GPIOB -> BSRR |= ((data/64%2) == 1) ? GPIO_BSRR_BS15  // 1
	   		                              : GPIO_BSRR_BR15;  // 1

	  GPIOB -> BSRR |= ((data/128%2) == 1) ? GPIO_BSRR_BS14  // 1
			                               : GPIO_BSRR_BR14;  // 0
*/

/*
 // Cortex M3에서만 동작하는거 = Cortex M4에서는 동작하지 않음
 // Cortex M4에서는 BSR 레지스터도 없음
  GPIOB -> BSRR |= ((data%2) == 1) ? (1 << Gpio_Out -> OUT_Bit_0)
		                          : (1 << (Gpio_Out -> OUT_Bit_0 + 16));

  GPIOC -> BSRR |= ((data/2%2) == 1) ? (1 << Gpio_Out -> OUT_Bit_1)
 		                            : ((1 << Gpio_Out -> OUT_Bit_1) << 16);

  GPIOC -> BSRR |= ((data/4%2) == 1) ? (1 << Gpio_Out -> OUT_Bit_2)
  		                            : (1 << (Gpio_Out -> OUT_Bit_2 + 16));

  GPIOA -> BSRR |= ((data/8%2) == 1) ? (1 << Gpio_Out -> OUT_Bit_3)
  		                            : ((1 << Gpio_Out -> OUT_Bit_3) << 16);

  GPIOB -> BSRR |= ((data/16%2) == 1) ? (1 << Gpio_Out -> OUT_Bit_4)
  		                             : ((1 << Gpio_Out -> OUT_Bit_4) << 16);

  GPIOB -> BSRR |= ((data/32%2) == 1) ? (1 << Gpio_Out -> OUT_Bit_5)
   		                             : (1 << (Gpio_Out -> OUT_Bit_5 + 16));

  GPIOB -> BSRR |= ((data/64%2) == 1) ? (1 << Gpio_Out -> OUT_Bit_6)
   		                             : ((1 << Gpio_Out -> OUT_Bit_6) << 16);

  GPIOB -> BSRR |= ((data/128%2) == 1) ? (1 << Gpio_Out -> OUT_Bit_7)
   		                              : (1 << (Gpio_Out -> OUT_Bit_7 + 16));
*/
}
//=======================================================

// 2025, 06, 19 추가 == AIOT4기 추가
struct GPIO_IN
{
  const uint16_t IN_Bit_0;
  const uint16_t IN_Bit_1;
  const uint16_t IN_Bit_2;
  const uint16_t IN_Bit_3;
  const uint16_t IN_Bit_4;
  const uint16_t IN_Bit_5;
  const uint16_t IN_Bit_6;
  const uint16_t IN_Bit_7;
}In = {
//		sw1_Pin,
//		sw2_Pin,
//		sw3_Pin,
//		sw4_Pin
//		//sw4_Pin,
		//sw5_Pin,
		//sw6_Pin,
		//sw7_Pin,
		//sw8_Pin   //GPIO_PIN_7
       }; // 핀 번호는 프로젝트에 맞게 수정하여 사용하세요
struct GPIO_IN *Gpio_In = &In;

// 2025, 06, 10 추가 = AIOT4기생 추가
//uint8_t gpio_sw[8] = {sw1_Pin, sw2_Pin,sw3_Pin,sw4_Pin,sw5_Pin,sw6_Pin,sw7_Pin,sw8_Pin};

// 2025, 06, 10 추가 = AIOT4기생 추가
int My_Port_Byte_Rd()
{
  uint16_t buf = 0;

  buf = In_Bit(GPIOA, Gpio_In -> IN_Bit_0) * 1;     // bit 0 = lsb
  buf |= In_Bit(GPIOA, Gpio_In -> IN_Bit_1) * 2;   // bit 1
  buf |= In_Bit(GPIOA, Gpio_In -> IN_Bit_2) * 4;   // bit 2
  buf |= In_Bit(GPIOB, Gpio_In -> IN_Bit_3) * 8;   // bit 3
  buf |= In_Bit(GPIOB, Gpio_In -> IN_Bit_4) * 16;  // bit 4
  buf |= In_Bit(GPIOB, Gpio_In -> IN_Bit_5) * 32;  // bit 5
  buf |= In_Bit(GPIOB, Gpio_In -> IN_Bit_6) * 64;  // bit 6
  buf |= In_Bit(GPIOB, Gpio_In -> IN_Bit_7) * 128; // bit 7 = msb

  return (buf << 8);
}

/*
// 2025, 06, 20 AIOT 4기 추가
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 GPIO_InitStruct.Pin = GPIO_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 GPIO_InitStruct.Pin = GPIO_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_PULLUP;
 HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


// Timer Interrupt 사용시 적용
void Delay_us(uint16_t time)
{
  __HAL_TIM_SET_COUNTER(&htim1,0);
  while(__HAL_TIM_GET_COUNTER(&htim1) < time);
}
*/

//=========================================================
//2019, 11, 08 추가
void Byte_out_L(GPIO_TypeDef *GPIOx, uint16_t out_data) // uint32_t = org = data sheet ��
{
  // Low 8bit out = default
  // ver 3.0
  uint16_t buf;

  buf =  GPIOx -> ODR; // GPIOx PORT PIN ���¸� ����
  buf &= 0xff00;
  GPIOx -> ODR = (buf | out_data);

  // old ver = v2.0
  // GPIOx -> ODR = (out_data & 0x00ff);
}

void Byte_out_H(GPIO_TypeDef *GPIOx, uint16_t out_data) // uint32_t = org = data sheet ��
{
   // High 8bit out
   // ver 3.0 = 2020,03,13 �߰�
   uint16_t buf;

   buf =  GPIOx -> ODR; // GPIOx PORT PIN ���¸� ����
   buf &= 0x00ff;
   out_data <<= 8;
   GPIOx -> ODR = (buf | out_data);

	/*
	// old ver = v2.0
	out_data <<= 8;
	GPIOx -> ODR = (out_data & 0xffff);
	*/
}

void Word_out(GPIO_TypeDef *GPIOx, uint16_t out_data)
{
   // 16 bit out
	GPIOx -> ODR = out_data;
}

void PortA_out_L(uint16_t out_data)
{
  // Low 8bit out = default
  // ver 3.0
  uint16_t buf;

  buf =  GPIOA -> ODR; // GPIOx PORT PIN ���¸� ����
  buf &= 0xff00;
  GPIOA -> ODR = (buf | out_data);
}

void PortA_out_H(uint16_t out_data)
{
  // High 8bit out
  // ver 3.0 = 2020,03,13 �߰�
  uint16_t buf;

  buf =  GPIOA -> ODR; // GPIOx PORT PIN ���¸� ����
  buf &= 0x00ff;
  out_data <<= 8;
  GPIOA -> ODR = (buf | out_data);
}

void PortA_out_WD(uint16_t out_data)
{
  // High 8bit out
  // ver 3.0 = 2020,03,13 �߰�

  // 16 bit out
  GPIOA -> ODR = out_data;
}

void PortB_out_L(uint16_t out_data)
{
  // Low 8bit out = default
  // ver 3.0
  uint16_t buf;

  buf =  GPIOB -> ODR; // GPIOx PORT PIN ���¸� ����
  buf &= 0xff00;
  GPIOB -> ODR = (buf | out_data);
}

void PortB_out_H(uint16_t out_data)
{
  // High 8bit out
  // ver 3.0 = 2020,03,13 �߰�
  uint16_t buf;

  buf =  GPIOB -> ODR; // GPIOx PORT PIN ���¸� ����
  buf &= 0x00ff;
  out_data <<= 8;
  GPIOB -> ODR = (buf | out_data);
}

void PortB_out_WD(uint16_t out_data)
{
  // High 8bit out
  // ver 3.0 = 2020,03,13 �߰�

  // 16 bit out
  GPIOB -> ODR = out_data;
}

void PortC_out_L(uint16_t out_data)
{
  // Low 8bit out = default
  // ver 3.0
  uint16_t buf;

  buf =  GPIOC -> ODR; // GPIOx PORT PIN ���¸� ����
  buf &= 0xff00;
  GPIOC -> ODR = (buf | out_data);
}



void PortC_out_H(uint16_t out_data)
{
  // High 8bit out
  // ver 3.0 = 2020,03,13 �߰�
  uint16_t buf;

  buf =  GPIOC -> ODR; // GPIOx PORT PIN ���¸� ����
  buf &= 0x00ff;
  out_data <<= 8;
  GPIOC -> ODR = (buf | out_data);
}

void PortC_out_WD(uint16_t out_data)
{
  // High 8bit out
  // ver 3.0 = 2020,03,13 �߰�

  // 16 bit out
  GPIOC -> ODR = out_data;
}

//========  In put  ========================
uint16_t Byte_rd_L(GPIO_TypeDef *GPIOx)
{
  // Low 8 bit input
  uint16_t buf;
  buf =  GPIOx -> IDR;
  return (buf & 0x00ff);
}

uint16_t Byte_rd_H(GPIO_TypeDef *GPIOx)
{
  // high 8bit input
  uint16_t buf;

  buf =  GPIOx -> IDR;
  return (buf & 0xff00);
}

uint16_t Word_rd(GPIO_TypeDef *GPIOx)
{
  // 16bit input = 2020, 03, 13 ����
  uint16_t buf;

  buf = GPIOx -> IDR;
  return buf;
}

uint16_t PortA_rd_L(void)
{
  // Low 8 bit input
  // ver 3.0
  uint16_t buf;

  buf =  GPIOA -> IDR;
  return (buf & 0x00ff);
}

uint16_t PortA_rd_H(void)
{
  // Low 8 bit input
  // ver 3.0
  uint16_t buf;

  buf =  GPIOA -> IDR;
  return (buf & 0xff00);
}

uint16_t PortA_rd_WD(void)
{
  int16_t buf;

  buf = GPIOA -> IDR;
  return buf;
}

uint16_t PortB_rd_L(void)
{
  // Low 8 bit input
  // ver 3.0
  uint16_t buf;

  buf =  GPIOB -> IDR;
  return (buf & 0x00ff);
}

uint16_t PortB_rd_H(void)
{
  // Low 8 bit input
  // ver 3.0
  uint16_t buf;

  buf =  GPIOB -> IDR;
  return (buf & 0xff00);
}

uint16_t PortB_rd_WD(void)
{
  int16_t buf;

  buf = GPIOB -> IDR;
  return buf;
}

uint16_t PortC_rd_L(void)
{
  // Low 8 bit input
  // ver 3.0
  uint16_t buf;

  buf =  GPIOC -> IDR;
  return (buf & 0x00ff);
}

uint16_t PortC_rd_H(void)
{
  // Low 8 bit input
  // ver 3.0
  uint16_t buf;

  buf =  GPIOC -> IDR;
  return (buf & 0xff00);
}

uint16_t PortC_rd_WD(void)
{
  int16_t buf;

  buf = GPIOC -> IDR;
  return buf;
}

//================================================
/*
 // ���� ������ �����ð��� ���� �Ϸ��Ͽ� ���� ����
  * 1. BSR / BR
  * 2. ODR, IDR
  * 3. std LIB, LL LIB, CMSIS LIB
  * 4. HAL LIB
  *
  * ���� ������ my GPIO LIB�� ����� ���� 2, 4�� �� ���� �մϴ�.
  * ������ �������� 1�� ��� ���� ������ ���� �Դϴ�.
  * 3�� ����� ����μ� �ð� �� ���ǻ� ������ ������ ���� �� �� ���̸� ���鼭
  * ���� �ϵ��� �ϰڽ��ϴ�.  1, 2, 4�� 3���� ��� ���� ���� ������ �帮��
  * ������ �� �Ŀ� ���� ������ ���� �ϰڽ��ϴ�.
  * my_gpio_Lib �� ���÷� ���׷��̵� �� ���� �Դϴ�. �������� Ȯ�ο�..
  * �ϴ� ��� �����ϴ� 1, 2, 4�� �� 1���� ������̶� ������ �Ͻñ� �ٶ��ϴ�.
  * ���⼭ 1�� �ϰ� 2���� �����ڵ�������� �������͸� ���� ������ ���� �մϴ�.= �ӵ��� �����ϴ�.
  * 3��, 4���� ���̺귯�� �������̶� �ӵ��� �����ϴ�.
  * ���� 1�� ����� �ӵ��� ���� �ٸ��ϴ�.
  * 4�� ����� �ӵ��� ���� �������.... �׷��ٰ� �����Ÿ��� ���� �����ų�
  * �츮 ���� ���� ���� ������ �ƴԴϴ� = AVR ���ٵ� �� �ʹ� �����ϴ�.
  *
#define bit_set(GPIO_TypeDef *GPIOx, uint16_t BIT) (GPIOx |= (1<<BIT)) // bit hi

#define bit_clr(GPIO_TypeDef *ADDRESS, uint16_t BIT) (ADDRESS &= ~(1<<BIT)) // bit low

//#define bit_chk(ADDRESS, BIT) (ADDRESS & (1<<BIT))  // bit chak

//#define tbi(PORT, BIT) (PORT ^= (1<<BIT))  //bit ���
 *


void GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) // ��Ʈ�� �ش� ���� Clear�Ѵ�.

  void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) // ��Ʈ�� �ش� ���� Set�Ѵ�.

  void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal) // ��Ʈ�� �ش� ���� ����.

  void GPIO_WriteBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, BitAction BitVal) // ��Ʈ�� Ư�� ��Ʈ�� 0 �Ǵ� 1�� ����.

*/

// 1. gpio output LIB test sample code
/*
void led_demo(void)
{
   out_port(GPIOA, R_LED_OUT_Pin | B_LED_OUT_Pin, 1);
   delay_ms(500); //3. System delay LIB test
   out_port(GPIOA, R_LED_OUT_Pin | B_LED_OUT_Pin, 0); // led 2
   delay_ms(500);

   out_tg(GPIOA, RGB_B_LED_OUT_Pin);
   delay_ms(500);
   out_port(R_LED_OUT_GPIO_Port, R_LED_OUT_Pin, 1);
   delay_ms(500);
   out_tg(GPIOA, R_LED_OUT_Pin | B_LED_OUT_Pin | RGB_B_LED_OUT_Pin );
   delay_ms(500);
   out_tg(BZ_OUT_GPIO_Port, BZ_OUT_Pin);

   out_port(RGB_R_LED_OUT_GPIO_Port, RGB_R_LED_OUT_Pin, Hi);
   HAL_Delay(200);
   out_port(RGB_R_LED_OUT_GPIO_Port, RGB_R_LED_OUT_Pin, Low);

   out_port(RBG_G_LED_OUT_GPIO_Port, RBG_G_LED_OUT_Pin, Hi);
   HAL_Delay(200);
   out_port(RBG_G_LED_OUT_GPIO_Port, RBG_G_LED_OUT_Pin, Low);

   out_port(RGB_B_LED_OUT_GPIO_Port, RGB_B_LED_OUT_Pin, 1);
   HAL_Delay(200);
   out_port(RGB_B_LED_OUT_GPIO_Port, RGB_B_LED_OUT_Pin, 0);

   HAL_Delay(200);
   out_port(RGB_R_LED_OUT_GPIO_Port, RGB_R_LED_OUT_Pin, Hi);
   out_port(RBG_G_LED_OUT_GPIO_Port, RBG_G_LED_OUT_Pin, GPIO_PIN_SET);
   out_port(RGB_B_LED_OUT_GPIO_Port, RGB_B_LED_OUT_Pin, 1);
   HAL_Delay(200);
   out_port(RGB_R_LED_OUT_GPIO_Port, RGB_R_LED_OUT_Pin, 0);
   out_port(RBG_G_LED_OUT_GPIO_Port,  RBG_G_LED_OUT_Pin, GPIO_PIN_RESET);
   out_port(RGB_B_LED_OUT_GPIO_Port, RGB_B_LED_OUT_Pin, Low);
   HAL_Delay(200);
   out_tg(BZ_OUT_GPIO_Port, BZ_OUT_Pin);
   HAL_Delay(200);
}
*/


//2. gpio input test LIB sample code
/*
void key_in(void)
{
	if(in_port(SW1_IN_GPIO_Port, SW1_IN_Pin) == 0)
	    {
		   out_port(GPIOA, LED1_Pin | LED2_RED_Led_Pin, 1);
	    }
	   else out_port(GPIOA, LED1_Pin | LED2_RED_Led_Pin, 0);

	   if(in_port(SW2_IN_GPIO_Port, SW2_IN_Pin) == 0)
	      {
		    out_port(RGB_R_Led_GPIO_Port, RGB_R_Led_Pin, 0);
		    out_port(RGB_G_Led_GPIO_Port, RGB_G_Led_Pin, GPIO_PIN_RESET);
		    out_port(RGB_B_Led_GPIO_Port, RGB_B_Led_Pin, Low);
	  	    led_demo();
	      }
	   else
	    {
		  out_port(RGB_R_Led_GPIO_Port, RGB_R_Led_Pin, Hi);
		  out_port(RGB_G_Led_GPIO_Port, RGB_G_Led_Pin, GPIO_PIN_SET);
		  out_port(RGB_B_Led_GPIO_Port, RGB_B_Led_Pin, 1);
	    }
}
*/


#endif
