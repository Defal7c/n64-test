OUTPUT_ARCH (mips)
SECTIONS
{
   __romPos = 0;
   _bootSegmentStart = ADDR(.boot); _bootSegmentRomStart = __romPos; .boot 0x04000000 : AT(__romPos)
   {
      asm/rom_header.o(.text);
      builds/boot.6102.o(.data);
   }
   _bootSegmentEnd = ADDR(.boot) + SIZEOF(.boot); _bootSegmentRomEnd = __romPos + SIZEOF(.boot); __romPos += SIZEOF(.boot);
   _codeSegmentStart = ADDR(.code); _codeSegmentRomStart = __romPos; .code 0x80000400 : AT(__romPos) SUBALIGN(16)
   {
      asm/entry.o(.text);
      main*.o(.text);
      */libultra_rom.a:*.o(.text);
      */libgcc.a:*.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.xbus.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.xbus.o(.text);
      main*.o(.*data*);
      */libultra_rom.a:*.o(.*data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.xbus.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.xbus.o(.data*);
      main*.o(.*rodata*);
      */libultra_rom.a:*.o(.*rodata*);
      */libgcc.a:*.o(.*rodata*);
   }
   _codeSegmentEnd = ADDR(.code) + SIZEOF(.code); _codeSegmentRomEnd = __romPos + SIZEOF(.code); __romPos += SIZEOF(.code);
   _codeSegmentBssStart = ADDR(.code.noload); .code.noload (NOLOAD) :
   {
      main*.o(.*bss*);
      */libultra_rom.a:*.o(COMMON);
      */libultra_rom.a:*.o(.scommon);
      */libultra_rom.a:*.o(.*bss*);
   }
   _codeSegmentBssEnd = ADDR(.code.noload) + SIZEOF(.code.noload); _codeSegmentBssSize = SIZEOF(.code.noload);
   /DISCARD/ :
   {
      *(*);
   }
}
