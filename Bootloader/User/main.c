#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "Bootloader.h"

int main(void)
{
	Bootloader_JumpTo_App(APPLICATION_START_ADDRESS);
}
