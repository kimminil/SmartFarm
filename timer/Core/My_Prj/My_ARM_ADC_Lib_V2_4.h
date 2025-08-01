/*
 * my_adc_lib_v2_2.h
 *
 *  송  명  규
 *  011 - 2402-4398
 *  mgsong@hanmail.net
 *
 *  V1.0 == 2015, 4, 8 = 최조작성
 *  V1.5 == 2020, 10, 24 = 모드추가
 *  V2.0 == 2023, 7, 2 = 기능 대폭보강
 *  V2.1 == 2023, 9, 25 = 함수추가
 *  V2.2 == 2024, 02, 06 = 구조체포인터 추가, 함수명 교체
 *  V2.3 == 2024, 08, 28 = 함수명 추가 및 코드 수정 보강
 *                       = IoT 8기 때 보강
 *  V2.4 == 2025, 02, 04 = displat 방식 변경 = AIOT 1기
 */


#ifndef __My_ADC_LIB__
#define __My_ADC_LIB__


#define  AVG_LOOP    82  //default = 32, 42, 82

// 기본 = 멀티채널, 연속모드, SW 트리거, end-ov-conversion, 12Bit
struct AD_Value
{
 uint16_t ad_loop;
 int8_t    ad_end_flag;
 uint16_t  ad_data;
 uint8_t  avg_loop; //82
 int8_t    ad_ch_nb;  // 1 = 싱글채널, 싱글스탭
 int8_t    ad_ch_cnt;
 uint16_t  ad_buf[20];
 uint16_t  volt;
 float     ad_f;
 float     volt_f[20];
 int8_t    ad_run_flag;
 uint32_t  test_loop;     // 2025, 01, 05 추가 == AIOT 1기
}adc_d = {0,0,0,82,1,0,0,0,0.0,0.0,0,0};
struct AD_Value *adc = &adc_d;

struct ACS712  // 전류센서 모듈 ==> SEN0214
{
  double   Voltage;
  double   Amps;
  int8_t   ACSoffset; // 기준 값 0A일때 아날로그 값은 2500mV 이다.
  uint8_t  mVperAmp; //5A = 185, 20A = 100, 30A = 66
  double smoothAmps;
}current = {0, 0, 2.5, 1, 0};
struct ACS712 *cur = &current;

#if dis_mid
  #include <stdio.h>
  #include <string.h>
  char adbuf[50];
#endif


// 켈리브레이션
void adc_Calibration(ADC_HandleTypeDef* adcHandle)
{
 if(adcHandle->Instance==ADC1){
   while(!HAL_ADC_PollForConversion(adcHandle, 50) != HAL_OK);
   printf("ADC Test OK\r\n\n");
  }
}

#define ad_ch_nb_set(value) adc -> ad_ch_nb = value;

void dis_Out(uint16_t abuf, char ch)
{
  char d_buf[60];
  uint16_t volt;
  float ad_f;

  //2025, 02, 05 추가
  (ch == 1) ? tx_str("Rank1, Ad_Ch9,  JOYST_Y, ", 2) :
  (ch == 2) ? tx_str("Rank2, Ad_Ch0,  Mult_VR, ", 2) :
  (ch == 3) ? tx_str("Rank3, Ad_Ch11, JOYST_X, ", 2) :
  (ch == 4) ? tx_str("Rank4, MCU_Tp,  MCUTemp, ", 2) :
  (ch == 5) ? tx_str("Rank5, Ad_Ch15, VR_AD  , ", 2) :
  (ch == 6) ? tx_str("Rank6, __MCU__, Temp_AD, ", 2) : 0;

  // 2025, 02, 04 추가
#if dis_hi
  printf("Test = %d, ADC Data = %d, Volt = %.4f\r\n", adc -> ad_buf[0], abuf, abuf * (3.3/4095));
#elif dis_mid
  // 방법 1
  sprintf(d_buf,"T_loop = %d, ADC Data = %d, Volt = %f\r\n", adc -> ad_buf[0], abuf, abuf * (3.3/4095));
  printf("%s", d_buf);

#elif dis_Low
  // Low data
  tx_str("ADC Data = ", 2);
  tx_send(adc -> ad_buf[0]/ 1000 + 0x30, 2);
  tx_send(adc -> ad_buf[0]% 1000/100 + '0', 2);
  tx_send(adc -> ad_buf[0]% 100/10 + 48, 2);
  tx_send(adc -> ad_buf[0]% 10 + 0x30, 2);
  tx_str(",  ", 2);

  // Low Lable ==>> FND?�� ?��?��?�� 경우
  ad_f = (abuf * (3.3/4095));//0.0008130081);
  volt = (int)(ad_f * 1000);

  tx_str("VOlt = ", 2);
  tx_send(volt/1000 + 0x30, 2);
  tx_send('.', 2);
  tx_send(volt%1000/100 + '0', 2);
  tx_send(volt%100/10 + 0x30, 2);
  tx_send(volt%10 + 0x30, 2);
  tx_str("\r\n", 2);
#endif
}

void Adc_Setup(uint8_t adc_ch_nb)
{
 memset(adc -> ad_buf, 0, 20 *sizeof(uint8_t));
 adc_Calibration(&hadc1);

#if ADC_Polling_Mode
    //ad ch set
    ad_ch_nb_set(adc_ch_nb);
    HAL_ADC_Start(&hadc1);
#elif ADC_IRQ_Mode
    //ad ch set
    ad_ch_nb_set(adc_ch_nb);
    adc -> ad_ch_cnt = 1;
    HAL_ADC_Start_IT(&hadc1);
#elif ADC_DMA_Mode
    //ad ch set
    ad_ch_nb_set(adc_ch_nb);
  //  HAL_ADC_Start_DMA(&hadc1, &adc -> ad_buf[1], adc -> ad_ch_nb);
#endif
}

#if ADC_Polling_Mode
// 폴링방식 모드

// scan Channel - Single Conversion Mode ==  Polling
void scan_ch_single_con(ADC_HandleTypeDef* adcHandle)
{
 //scan Channel - Single Single Conversion == Polling
 for(int loop = 1; loop <= adc -> ad_ch_nb; loop++)
  {
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);  //busy
	adc -> ad_buf[loop] = HAL_ADC_GetValue(&hadc1);
	printf("Tp = %d, ADC Data = %d, Volt = %f\r\n", adc -> ad_buf[0], adc -> ad_buf[loop], adc -> ad_buf[loop] * (3.3/4095));
	HAL_Delay(500);
 }
printf("\r\n");

adc -> test_loop++;
adc -> ad_buf[0] = adc -> test_loop;
HAL_ADC_Stop(&hadc1);
//memset(adc -> ad_buf, 0, 20 *sizeof(uint16_t));
}


// 2024, 08, 29 추가 및 수정 보강
// Single Channel - Single Contiinuous Mode ==  Polling
void Polling_single_ch_continu_con(ADC_HandleTypeDef* adcHandle)
{
  HAL_ADC_Start(adcHandle);
  HAL_ADC_PollForConversion(adcHandle, 100);
  adc -> ad_buf[0] = HAL_ADC_GetValue(adcHandle); //hadc1
  //HAL_ADC_Stop(&hadc1);

  // 2025m 02, 04 추가
#if dis_Low
  tx_str("AD data  = ", 2);
  tx_send(adc -> ad_buf[0]/ 1000 + 0x30, 2);
  tx_send(adc -> ad_buf[0]% 1000/100 + '0', 2);
  tx_send(adc -> ad_buf[0]% 100/10 + 48, 2);
  tx_send(adc -> ad_buf[0]% 10 + 0x30, 2);

  adc -> volt = (adc -> ad_buf[0] * ((3.3)/4095) * 1000);// 정규화
  tx_str(",  Volt = ", 2);
  tx_send(adc -> volt/ 1000 + 0x30, 2);
  tx_send('.', 2);
  tx_send(adc -> volt% 1000/100 + '0', 2);
  tx_send(adc -> volt% 100/10 + 48, 2);
  tx_send(adc -> volt% 10 + 0x30, 2);
  tx_str("\r\n", 2);
#elif dis_mid
  sprintf(adbuf,"ADC Data = %d, Volt = %f\r\n", adc -> ad_buf[0], adc -> ad_buf[0] * (3.3/4095));
  //tx_str(adbuf, 2);
  printf("%s", adbuf);
#elif dis_hi
  printf("ADC Data = %d, Volt = %f\r\n", adc -> ad_buf[0], adc -> ad_buf[0] * (3.3/4095));
#endif
}

// 2024, 08, 28 추가 및 수정 보강
// Single Channel - Single Conversion Mode ==  Polling
void Polling_single_ch_single_con(ADC_HandleTypeDef* adcHandle)
 {
  HAL_ADC_Start(adcHandle);  //Single Channel, Single Conversion일때   //(&hadc1);
  HAL_ADC_PollForConversion(adcHandle, 100);
  adc -> ad_buf[0] = HAL_ADC_GetValue(adcHandle); //hadc1
  HAL_ADC_Stop(adcHandle);

  // 2025m 02, 04 추가
#if dis_Low
  tx_str("ADC Data = ", 2);
  tx_send(adc -> ad_buf[0]/ 1000 + 0x30, 2);
  tx_send(adc -> ad_buf[0]% 1000/100 + '0', 2);
  tx_send(adc -> ad_buf[0]% 100/10 + 48, 2);
  tx_send(adc -> ad_buf[0]% 10 + 0x30, 2);

  adc -> volt = (adc -> ad_buf[0] * ((3.3)/4095) * 1000);// 정규화
  tx_str(",  Volt = ", 2);
  tx_send(adc -> volt/ 1000 + 0x30, 2);
  tx_send('.', 2);
  tx_send(adc -> volt% 1000/100 + '0', 2);
  tx_send(adc -> volt% 100/10 + 48, 2);
  tx_send(adc -> volt% 10 + 0x30, 2);
  tx_str("\r\n", 2);
#elif dis_mid
  sprintf(adbuf,"ADC Data = %d, Volt = %f\r\n", adc -> ad_buf[0], adc -> ad_buf[0] * (3.3/4095));
  //tx_str(adbuf, 2);
  printf("%s", adbuf);
#elif dis_hi
  printf("ADC Data = %d, Volt = %f\r\n", adc -> ad_buf[0], adc -> ad_buf[0] * (3.3/4095));
#endif
}

// Mult_Ch Single & Contiinuous Conversiond == Polling
void adc_Mult_Ch_Polling(ADC_HandleTypeDef* adcHandle, uint32_t *adc_p, int8_t adc_ch_nb)
{
   //HAL_ADC_Start(adcHandle);  // Scan Channel, Contiinuous Conversiondl일때
  for(int i = 1; i <= adc_ch_nb; i++)
    {
	  HAL_ADC_Start(adcHandle); //Scan Channel, Single Conversion일때   //(&hadc1);
	  HAL_ADC_PollForConversion(adcHandle, 100);
	  adc_p[i] = HAL_ADC_GetValue(adcHandle); //hadc1

	  adc -> test_loop++;
	  adc -> ad_buf[0] = adc -> test_loop;

	  dis_Out(adc_p[i], i);
	  HAL_Delay(500);
    }
  printf("\r\n");

  HAL_ADC_Stop(adcHandle);
}


// Mult_Ch Single & Contiinuous Conversiond == Polling
// 8번 평균 낸거
void adc_Polling_avg(ADC_HandleTypeDef* adcHandle, uint32_t *adc_p, int8_t adc_ch_nb, int8_t avg_loop)
{
  int8_t i, k;

  // buff init
  for(i = 1; i <= adc_ch_nb; i++)
	  adc_p[i]=0;

  for(k = 1; k <= avg_loop; k++)	// avg
   {
     for(i = 1; i <= adc_ch_nb; i++)
      {
    	HAL_ADC_Start(adcHandle); //&hadc1);
        HAL_ADC_PollForConversion(adcHandle, 100); //&hadc,
        adc_p[i] += HAL_ADC_GetValue(adcHandle);   //(&hadc);
        HAL_Delay(1);
      }
     HAL_ADC_Stop(adcHandle); //(&hadc1);
   }
  adc -> test_loop++;
  adc_p[0] = adc -> test_loop;

  for(i =1; i <= adc_ch_nb; i++)
   {
    adc_p[i] = (adc_p[i] / avg_loop);
    dis_Out(adc_p[i], i);
    HAL_Delay(10);
   }
  printf("\r\n");
}



//==================================================
// ADC IRQ Mode,  // ADC DMA Mode
#elif ADC_IRQ_Mode || ADC_DMA_Mode
     // adc Channel set == // 1 = 싱글채널

#endif


void XferCpltCallback(hdma)
{
 // xxIT.C ADC DMA 핸들러에 콜백함수를 선언 해줘야하고
 //	== 3개 채널만 나옴
 adc -> ad_end_flag = 1;
 HAL_ADC_Stop_DMA(&hadc1);
}

// ADC Call back func
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adcHandle)
{
 // xxIT.C ADC DMA 핸들러에 콜백함수를 선언 안 해줘도 됨
 // 전 채널 다 나옴 == 값도 잘 나옴
 // 가급적 이 콜백함수 사용

#if ADC_DMA_Mode
	 //adc -> ad_end_flag = 1;
	 ad_end_flag1 = 1; //rtos = dma방식
	 HAL_ADC_Stop_DMA(&hadc1);

#elif ADC_IRQ_Mode
	 if (__HAL_ADC_GET_FLAG(adcHandle, ADC_FLAG_EOC))
	  {
		 adc -> ad_buf[adc -> ad_ch_cnt] = HAL_ADC_GetValue(adcHandle);
		 adc -> ad_ch_cnt++;
		 if(adc -> ad_ch_cnt > adc -> ad_ch_nb) // org = 1
		 {
		   HAL_ADC_Stop_IT(adcHandle);
		   adc -> ad_ch_cnt = 1;
		   adc -> ad_end_flag = 1;
		 }
		else  HAL_ADC_Start_IT(&hadc1);


		// 셀프 test 용
	   // printf("ADC Data = %d, Volt = %f\r\n", adc -> ad_buf[0], adc -> ad_buf[0] * (3.3/4095));
	   // HAL_ADC_Start_IT(adcHandle);
	   }

//=======================================================
/*
    // Test ex
	adc.ad_buf[adc.ad_ch_cnt] = 0;
	 if(adc.ad_ch_cnt > adc.ad_ch) //4ch
	  {
		 HAL_ADC_Stop_DMA(hadc1);
		 adc.ad_ch_cnt = 1;
		 adc.ad_end_flag = 1;
	  }
	 else
	 {
	   adc.ad_buf[adc.ad_ch_cnt] = HAL_ADC_GetValue(hadc1);
	   adc.ad_ch_cnt++;

	   HAL_ADC_Start_IT(hadc1);
	 }
 */
#endif
}

#endif /* INC_MY_LIB_MY_ADC_LIB_V2_0_H_ */

