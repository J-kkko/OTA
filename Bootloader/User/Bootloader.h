#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

typedef void (*pcFunction)(void); // 定义函数指针类型

void Bootloader_JumpTo_App(uint32_t appVectorTableAddr);

#endif
