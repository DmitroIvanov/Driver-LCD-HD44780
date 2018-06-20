//hd44780.c
//функции для работы с LCD HD44780
/*----работа с библиотекой-----------
1. инициировать GPIO - void gpio_lcd_init(void);
2. инициировать дисплей - void LcdInit(void);
3. После указаных процедур можно отправлять символы в кодировке ASCII 
	на индикацию - void SendData(uint8_t data);
	символ появится на месте курсора.
4. Если символ нужно вывести в определенное место, то перед отправкой символа нужно указать адрес 
	где его высветить с помощью - void SetAddressDDRAM(uint8_t address);
5. Все функции соответствуют ф-циям описаным в Даташите на индикатор.
*/
//********************************************************
#include <hd44780.h>
//********************************************************
void gpio_lcd_init(void)
{
//--------GPIO_Init---------------------------------
	GPIO_RCC;
	E_init; RS_init;
	DB4_init; DB5_init; DB6_init; DB7_init;
	
//-----Исходное положене ног ------------------------------
	RS_1; E_1;
	DB4_1; DB5_1; DB6_1; DB7_1;

//----инициализация таймера для реализации задержек-------------------
	TIM_RCC_CLK;
	TIM_DELAY->PSC = 48-1; //делитель чтоб таймер тикал 1 microsecond
	TIM_DELAY->CR1 |= TIM_CR1_OPM; //реж одиночного имп
}
//*******************************************************************
#define DELAY_MAX_VALUE 0xFFFF //МАХ значение времени задержки, не должно превышать 65 535 us
void delay_us(uint16_t us)
{
 	if (us > DELAY_MAX_VALUE)//если заданое значение задержки больше макс возможного
 	{//то устанавливаем максим задержку
 		us = 0xFFFF;
 	}
 	TIM_DELAY->ARR = us;// значение задержки в мkсек (us+1).
 	TIM_DELAY->CNT = 0;//
 	TIM_DELAY->CR1 |= TIM_CR1_CEN;
 	while((TIM_DELAY->SR & TIM_SR_UIF) == 0){}
 	TIM_DELAY->SR &= ~TIM_SR_UIF;	
}
//*******************************************************************
//1. очистка экрана и установкa курсора в ноль
void ScreenClear(void)//
{
	RS_0;//передаем команду
	DB7_0; DB6_0; DB5_0; DB4_0;//старший полубайт
	E_1; delay_us(100);
	E_0; delay_us(100);
	DB7_0; DB6_0; DB5_0; DB4_1;//младший полубайт
	E_1; delay_us(100);
	E_0; delay_us(4000);//необходимая задержка
}
//---------------------------------------------------------
//2. возврат курсора в нулевую позицию
void CursorZero(void)//
{
	RS_0;//передаем команду
	DB7_0; DB6_0; DB5_0; DB4_0;//старший полубайт
	E_1; delay_us(100);
	E_0; delay_us(100);
	DB7_0; DB6_0; DB5_1; DB4_0;//младший полубайт
	E_1; delay_us(100);
	E_0; delay_us(4000);
}
//---------------------------------------------------------
//3. направление сдвига курсора и экрана при вводе символов
//ID=1 - при записи следующего символа курсор сдвигается вправо
//ID=0 - при записи следующего символа курсор сдвигается влево
//S=1 - или сдвигается экран
//S=0 - сдвиг экрана запрещен
void EntryModeSet(uint8_t ID, uint8_t S)//
{
	RS_0;//передаем команду
	DB7_0; DB6_0; DB5_0; DB4_0;//старший полубайт
	E_1; delay_us(100);
	E_0; delay_us(100);
	DB7_0; DB6_1;
	if(ID) 	{DB5_1;}
	else	{DB5_0;}
	if(S) 	{DB4_1;}
	else	{DB4_0;}
	E_1; delay_us(100);
	E_0; delay_us(4000);//необходимая задержка
}
//---------------------------------------------------------
//4. вкл/выкл экрана
//ScreenOffOn=1 - дисплей вкл
//ScreenOffOn=0 - дисплей выкл
//CursorOffOn=1 - курсор вкл
//CursorOffOn=0 - курсор выкл
//CursorBlink=1 - знакоместо над курсором моргает
//CursorBlink=1 - знакоместо над курсором не моргает
void ScreenCursorOffOn(uint8_t ScreenOffOn, uint8_t CursorOffOn, uint8_t CursorBlink)
{
	RS_0;//передаем команду
	DB7_0; DB6_0; DB5_0; DB4_0;//старший полубайт
	E_1; delay_us(100);
	E_0; delay_us(100);
	DB7_1;
	delay_us(100);
	if(ScreenOffOn) {DB6_1;}
	else			{DB6_0;}
	if(CursorOffOn) {DB5_1;}
	else			{DB5_0;}
	if(CursorBlink) {DB4_1;}
	else			{DB4_0;}	
	E_1; delay_us(100);
	E_0; delay_us(4000);//необходимая задержка
}
//---------------------------------------------------------
//5. двигаем курсор или экран
//ScreenOrCursor=1 - двигаем экраном
//ScreenOrCursor=0 - двигаем курсором
//LeftOrRight=1 - двигаем вправо
//LeftOrRight=0 - двигаем влево
void ScreenCursorShift(uint8_t ScreenOrCursor, uint8_t LeftOrRight)
{
	RS_0;//передаем команду
	DB7_0; DB6_0; DB5_0; DB4_1;//старший полубайт
	E_1; delay_us(100);
	E_0; delay_us(100);
	if(ScreenOrCursor) {DB7_1;}
	else			{DB7_0;}
	if(LeftOrRight) {DB6_1;}
	else			{DB6_0;}
	DB5_0;	DB4_0;
	E_1; delay_us(100);
	E_0; delay_us(4000);//необходимая задержка
}
//---------------------------------------------------------
//6. установка разрядности дисплея
//DL=1 - 8-bit
//DL=0 - 4-bit
//N=1 - 2 строки
//N=0 - 1 строка
//F=1 - 5x8 pix
//F=0 - 5x10 pix
void FunctionSet(uint8_t DL, uint8_t N, uint8_t F)
{
	RS_0;//передаем команду
	DB7_0; DB6_0; DB5_1;
	if(DL) 	{DB4_1;}
	else	{DB4_0;}
	E_1; delay_us(100);
	E_0; delay_us(100);
	if(N)	{DB7_1;}
	else	{DB7_0;}
	if(F) 	{DB6_1;}
	else	{DB6_0;}
	DB5_0;	DB4_0;
	E_1; delay_us(100);
	E_0; delay_us(4000);//необходимая задержка
}
void FunctionSet_8(void)
{//утановить 8-разрядный реж работы
	RS_0;//передаем команду
	DB7_0; DB6_0; DB5_1; DB4_1;
	E_1; delay_us(100);
	E_0; delay_us(4000);
}
//---------------------------------------------------------
//7. установка адреса в памяти CGRAM
void SetAddressCGRAM(uint8_t address)
{
	RS_0;//передаем команду
	DB7_0; DB6_1;
	if(address & 0x20) 	{DB5_1;}
	else				{DB5_0;}
	if(address & 0x10) 	{DB4_1;}
	else				{DB4_0;}
	E_1; delay_us(100);
	E_0; delay_us(100);
	if(address & 0x08) 	{DB7_1;}
	else				{DB7_0;}
	if(address & 0x04) 	{DB6_1;}
	else				{DB6_0;}
	if(address & 0x02) 	{DB5_1;}
	else				{DB5_0;}
	if(address & 0x01) 	{DB4_1;}
	else				{DB4_0;}
	E_1; delay_us(100);
	E_0; delay_us(4000);
}
//---------------------------------------------------------
//8. установка адреса в памяти DDRAM
void SetAddressDDRAM(uint8_t address)
{
	RS_0;//передаем команду
	DB7_1;
	if(address & 0x40) 	{DB6_1;}
	else				{DB6_0;}
	if(address & 0x20) 	{DB5_1;}
	else				{DB5_0;}
	if(address & 0x10) 	{DB4_1;}
	else				{DB4_0;}
	E_1; delay_us(100);
	E_0; delay_us(100);
	if(address & 0x08) 	{DB7_1;}
	else				{DB7_0;}
	if(address & 0x04) 	{DB6_1;}
	else				{DB6_0;}
	if(address & 0x02) 	{DB5_1;}
	else				{DB5_0;}
	if(address & 0x01) 	{DB4_1;}
	else				{DB4_0;}
	E_1; delay_us(100);
	E_0; delay_us(4000);
}
//---------------------------------------------------------
//9. отправка данных по указанному выше адрессу
//uint8_t data - ASCII код символа выводимого на экран
void SendData(uint8_t data)
{
	RS_1;//передаем данные
	if(data & 0x80) 	{DB7_1;}
	else				{DB7_0;}
	if(data & 0x40) 	{DB6_1;}
	else				{DB6_0;}
	if(data & 0x20) 	{DB5_1;}
	else				{DB5_0;}
	if(data & 0x10) 	{DB4_1;}
	else				{DB4_0;}
	E_1; delay_us(100);
	E_0; delay_us(100);
	if(data & 0x08) 	{DB7_1;}
	else				{DB7_0;}
	if(data & 0x04) 	{DB6_1;}
	else				{DB6_0;}
	if(data & 0x02) 	{DB5_1;}
	else				{DB5_0;}
	if(data & 0x01) 	{DB4_1;}
	else				{DB4_0;}
	E_1; delay_us(100);
	E_0; delay_us(4000);//необходимая задержка
}
//*******************************************************************
//----инициализация индикатора---------------------
void LcdInit(void)//
{
	delay_us(20000);
	FunctionSet_8();
	FunctionSet(0, 1, 1);
	FunctionSet(0, 1, 1);//4-razrada, курсор вкл, место моргает
	ScreenCursorOffOn(0, 0, 0);//экран выкл
	ScreenClear();//очитка экрана, курсор в ноль
	EntryModeSet(1, 0);//курсор сдвигается вправо при вводе, экран не двигается
	ScreenCursorOffOn(1, 1, 1);//экран вкл, курсор вкл и мигает
}
//******************************************************************
