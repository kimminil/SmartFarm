
/*
  2024, 12, 23
  송명규
  010-2402-4398
  mgsong@hanmail.net 

  V1.0 == 2024, 12, 23 = 최초작성 = AIOT 8기
  
*/



#ifndef __ Mult_Func_Board__
#define __ Mult_Func_Board__


enum _LED_
{
  m_led1 = 10,
  m_led2 = 11,
  m_led3 = 12,
  m_led4 = 13
}led_m;
char M_LED[5] = {m_led1, m_led2, m_led3, m_led4};

enum _KEY_M
{
  SW1 = A1,
  SW2 = 16,
  SW3 = A3
}key_m;

#define  BZ  3
#define  VR  A0

enum _FND_
{
  LATCH = 4,
  CLK = 7,
  DATA = 8
}fnd;

#endif



