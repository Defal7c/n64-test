include /usr/include/n64/make/PRdefs

NUSYSINC = /usr/include/n64/nusys
NUSYSLIB = /usr/lib/n64/nusys

BUILD_DIR = builds
LIB = /usr/lib/n64
LINC = $(LIB)/PR
INC = /usr/include/n64

LCDEFS  = -DF3DEX_GBI_2
LCINCS  = -I. -I$(NUSYSINC) -I/usr/include/n64/PR
LCOPTS  = -G 0
LDIRT   = $(ELF) $(ASMOBJECTS) $(MAP) $(TARGETS)
LDFLAGS = -L/usr/lib/n64 $(N64LIB) -L $(N64_LIBGCCDIR)
#LDFLAGS = $(MKDEPOPT) -L$(LIB) -L$(NUSYSLIB) -lnusys -lultra_rom -L$(N64_LIBGCCDIR) -lgcc
CPPFLAGS := -P -Wno-trigraphs $(LCINCS)

OPTIMIZER = -g

ELF		= test2.elf
TARGETS = ../test2.z64
MAP  	= test2.map
LD_SCRIPT = test2.ld
CP_LD_SCRIPT = $(BUILD_DIR)/test2_cp.ld
CP_LD_SCRIPT = test2_cp.ld

ASMFILES := $(wildcard asm/*.s)
ASMOBJS  = $(ASMFILES:.s=.o)

CODEFILES := $(wildcard *.c)
SCREENFILES := $(wildcard screens/*.c)
DATAFILES   := $(wildcard data/*.c)
CODEOBJS 	 = $(CODEFILES:.c=.o) $(SCREENFILES:.c=.o) $(DATAFILES:.c=.o) $(NUSYSLIB)/nusys_rom.o

DATAFILES2 =
DATAOBJS   = $(DATAFILES2:.c=.o)

CODESEGMENT = $(BUILD_DIR)/codesegment.o

BOOT	 = /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ = $(BUILD_DIR)/boot.6102.o

OBJS = $(CODESEGMENT) $(DATAOBJS) $(ASMOBJS) $(BOOT_OBJ) 

default: $(TARGETS)

cleaner:
		find . -name '*.v64' -delete
		find . -name '*.z64' -delete
		find . -name '*.elf' -delete
		find . -name '*.o' -delete
		find . -name '*.bin' -delete
		find . -name '*.plan_bak' -delete
		find . -name '*.sprite' -delete
		find . -name '*.dfs' -delete
		find . -name '*.raw' -delete
		find . -name '*.gen.h' -delete
		rm -f builds/*.z64 builds/*.n64

include $(COMMONRULES)

.s.o:
		$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT): $(CODEOBJS) Makefile
		  $(LD) -o $(CODESEGMENT) -r $(CODEOBJS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
		$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
		#cpp $(CPPFLAGS) -DBUILD_DIR=$(BUILD_DIR) -MMD -MP -MT $@ -MF $@.d -o $@ $<
		cpp $(CPPFLAGS) -MMD -MP -MT $@ -MF $@.d -o $@ $<

$(ELF): $(OBJS) $(CP_LD_SCRIPT)
		$(LD) -L$(BUILD_DIR) -T $(CP_LD_SCRIPT) -Map $(MAP) -o $@

$(TARGETS): $(ELF)
		$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $< $@ -O binary
		makemask $@

