/* forked from lkscript from CrashOverdrive */
/* also stefanmielke on ultra64-templates again */
OUTPUT_ARCH (mips)

#include <nusys.h>

/* Starts a segment */
#define BEGIN_SEG(name, addr) \
    _##name##SegmentStart = ADDR(.name); \
    _##name##SegmentRomStart = __romPos; \
    .name addr : AT(__romPos)

/* Ends a segment */
#define END_SEG(name) \
    _##name##SegmentEnd = ADDR(.name) + SIZEOF(.name); \
    _##name##SegmentRomEnd = __romPos + SIZEOF(.name); \
    __romPos += SIZEOF(.name);

/* Starts a noload segment which is intended to contain variables which do not have data yet. */
#define BEGIN_NOLOAD(name) \
    _##name##SegmentBssStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

/* Ends a noload segment */
#define END_NOLOAD(name) \
    _##name##SegmentBssEnd = ADDR(.name.noload) + SIZEOF(.name.noload); \
    _##name##SegmentBssSize = SIZEOF(.name.noload);

SECTIONS
{
   __romPos = 0;

   BEGIN_SEG(boot, 0x04000000)
   {
      asm/rom_header.o(.text); /* ROM Header */
      builds/boot.6102.o(.data); /* CIC 6102 bootcode */
   }
   END_SEG(boot)

   BEGIN_SEG(code, 0x80000400) SUBALIGN(16)
   {
      asm/entry.o(.text);
      main*.o(.text);

      */libultra_rom.a:*.o(.text);
      */libgcc.a:*.o(.text);

      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.xbus.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.xbus.o(.text);

      /* data */
      main*.o(.*data*);

      */libultra_rom.a:*.o(.*data*);
      
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.xbus.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.xbus.o(.data*);

      /* rodata */
      main*.o(.*rodata*);

      */libultra_rom.a:*.o(.*rodata*);
      */libgcc.a:*.o(.*rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code) 
   {
      main*.o(.*bss*);

      */libultra_rom.a:*.o(COMMON);
      */libultra_rom.a:*.o(.scommon);
      */libultra_rom.a:*.o(.*bss*);
   }
   END_NOLOAD(code)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
