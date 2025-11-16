#include "i2c_bitbang.h"
#include "lcd1602_i2c.h"

int main(void)
{
    I2C_Init();
    LCD_Init();

    LCD_SetCursor(0, 0);
    LCD_SendString("Hello Myhailo!");

    LCD_SetCursor(0, 1);
    LCD_SendString("Soft I2C works!");

    while(1);
}
