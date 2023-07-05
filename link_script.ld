ENTRY(main_func)
  
SECTIONS
{
    . = 0x7C00;    /* 加载地址 */

    .text : AT(0x7C00)    /* text 段位置 */
    {
        *(.text)
    }

    .sig : AT(0x7DFE)    /* 引导扇区签名位置 */
    {
        SHORT(0xAA55)
    }

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
    }

    . = 0x7E00;  /* 确保我们有 512 字节的空间 */
}
