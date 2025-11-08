#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "Bootloader.h"

void Bootloader_JumpTo_App(uint32_t appVectorTableAddr)
{
    pcFunction appResetHandler; // App复位函数指针
    uint32_t appStackTop;      // App栈顶地址

    // 验证App栈顶地址有效性（需在RAM范围内）
    appStackTop = *(volatile uint32_t *)appVectorTableAddr;
    if (appStackTop < RAM_START_ADDRESS || appStackTop > RAM_END_ADDRESS)
    {
        return; // 无效地址，直接返回
    }

    // 设置栈顶指针
    __set_MSP(appStackTop);

    // 获取App复位处理函数地址
    appResetHandler = (pcFunction)(*(volatile uint32_t *)RESET_HANDLER_ADDRESS);
    // 跳转到App复位处理函数
    appResetHandler();
}
