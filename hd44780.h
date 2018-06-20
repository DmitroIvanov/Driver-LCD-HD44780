//hd44780.h
//********************************************************
#ifndef __HD44780_H__
#define __HD44780_H__

#include <stm32f0xx.h>
#include <system_stm32f0xx.h>
#include "string.h"
#include "stdlib.h"
//********************************************************
//------инициализация GPIO------------------------
#define GPIO_RCC		RCC->AHBENR |= RCC_AHBENR_GPIOAEN

#define E_init			GPIOA->MODER |= GPIO_MODER_MODER4_0
#define RS_init			GPIOA->MODER |= GPIO_MODER_MODER5_0
#define DB4_init		GPIOA->MODER |= GPIO_MODER_MODER0_0
#define DB5_init		GPIOA->MODER |= GPIO_MODER_MODER1_0
#define DB6_init		GPIOA->MODER |= GPIO_MODER_MODER2_0
#define DB7_init		GPIOA->MODER |= GPIO_MODER_MODER3_0

//------вкл + выкл разрадов индикатора + управляющ имп------------------------
#define E_0		GPIOA->BSRR |= GPIO_BSRR_BR_4
#define E_1		GPIOA->BSRR |= GPIO_BSRR_BS_4

#define RS_0	GPIOA->BSRR |= GPIO_BSRR_BR_5
#define RS_1	GPIOA->BSRR |= GPIO_BSRR_BS_5

#define DB4_0	GPIOA->BSRR |= GPIO_BSRR_BR_0
#define DB4_1   GPIOA->BSRR |= GPIO_BSRR_BS_0

#define DB5_0	GPIOA->BSRR |= GPIO_BSRR_BR_1
#define DB5_1   GPIOA->BSRR |= GPIO_BSRR_BS_1

#define DB6_0	GPIOA->BSRR |= GPIO_BSRR_BR_2
#define DB6_1   GPIOA->BSRR |= GPIO_BSRR_BS_2

#define DB7_0	GPIOA->BSRR |= GPIO_BSRR_BR_3
#define DB7_1   GPIOA->BSRR |= GPIO_BSRR_BS_3

//инициализация таймера для реализации задержек без прерывания
#define TIM_DELAY	   TIM14    //какой таймер используем для задержки
#define TIM_RCC_CLK	   (RCC->APB1ENR |= RCC_APB1ENR_TIM14EN)  //вкл тактирование этого таймера

//********************************************************
void gpio_lcd_init(void);//инициализация GPIO и установка их в исходное положение

void LcdInit(void);//инициализация дисплея
//********************************************************
//функции работы с дисплеем:
void ScreenClear(void);//1. очистка экрана и установкa курсора в ноль
void CursorZero(void)//2. возврат курсора в нулевую позицию
void EntryModeSet(uint8_t ID, uint8_t S);//3. направление сдвига курсора и экрана при вводе символов
void ScreenCursorOffOn(uint8_t ScreenOffOn, uint8_t CursorOffOn, uint8_t CursorBlink);//4. вкл/выкл экрана
void ScreenCursorShift(uint8_t ScreenOrCursor, uint8_t LeftOrRight);//5. двигаем курсор или экран
void FunctionSet(uint8_t DL, uint8_t N, uint8_t F);//6. установка разрядности дисплея
void SetAddressCGRAM(uint8_t address);//7. установка адреса в памяти CGRAM
void SetAddressDDRAM(uint8_t address);//8. установка адреса в памяти DDRAM
void SendData(uint8_t data);//9. отправка данных по указанному выше адрессу
//********************************************************

#endif /* __HD44780_H__ */
