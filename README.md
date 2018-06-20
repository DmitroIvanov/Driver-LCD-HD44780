# Driver-LCD-HD44780
Драйвер для LCD-HD44780

Функции для работы с LCD HD44780
----работа с библиотекой-----------
1. Инициировать GPIO - void gpio_lcd_init(void);
2. Инициировать дисплей - void LcdInit(void);
3. После указаных процедур можно отправлять символы в кодировке ASCII на индикацию - void SendData(uint8_t data);
	Символ появится на месте курсора.
4. Если символ нужно вывести в определенное место, то перед отправкой символа нужно указать адрес где его высветить с помощью - void SetAddressDDRAM(uint8_t address);
5. Все функции соответствуют ф-циям описаным в Даташите на индикатор.