# PROJECT CONFIGURATION
OPT = -O3

DEFINE = \
	__HEAP_SIZE=8192 \
	__STACK_SIZE=8192 \
	NRF52832_XXAA \
	DEBUG

SRCDIR = \
	source

INCDIR = \
	header

LIB_SRC = \
	gcc_startup_nrf52.S \
	system_nrf52.c

LDDESC = nrf52_xxaa.ld

# BUILD CONFIGURATION
TARGET = application
OUTDIR = build
OBJDIR = objects
DEPDIR = depends

DEBUG = -g3
VPATH := $(SRCDIR) $(N5_MDK)
INCDIR += $(N5_MDK) $(CMSIS)
ARCHIVE += c nosys m

INCDIR := $(addprefix -I,$(INCDIR))
DEFINE := $(addprefix -D,$(DEFINE))
ARCHIVE := $(addprefix -l,$(ARCHIVE))

MACHINE = \
	-mcpu=cortex-M4 \
	-mthumb \
	-mabi=aapcs \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \

ASMFLAGS := \
	$(MACHINE) \
	$(DEBUG) \
	$(DEFINE) \
	$(INCDIR)

CFLAGS := \
	-std=c99 \
	-ffunction-sections \
	-fdata-sections \
	-fno-strict-aliasing \
	-fno-builtin \
	-fshort-enums \
	$(MACHINE) \
	$(OPT) \
	$(DEBUG) \
	$(DEFINE) \
	$(INCDIR)

LDFLAGS := \
	-Wl,--gc-sections \
	--specs=nano.specs \
	$(MACHINE) \
	$(ARCHIVE) \
	-L$(N5_MDK) \
	-T$(LDDESC)

# BUILD PREPARATION
ASM := $(OUTDIR)/$(TARGET).asm
HEX := $(OUTDIR)/$(TARGET).hex
BIN := $(OUTDIR)/$(TARGET).bin
MAP := $(OUTDIR)/$(TARGET).map
OUT := $(OUTDIR)/$(TARGET).out

EXTS = .c .cpp .S
SOURCES := $(foreach DIR,$(SRCDIR),$(subst $(DIR)/,,$(wildcard $(addprefix $(DIR)/*,$(EXTS)))))
OBJECTS := $(addprefix $(OUTDIR)/$(OBJDIR)/,$(addsuffix .o,$(SOURCES) $(LIB_SRC)))
DEPENDS := $(addprefix $(OUTDIR)/$(DEPDIR)/,$(addsuffix .d,$(SOURCES) $(LIB_SRC)))

BADOBJS := $(filter-out $(OBJECTS),$(wildcard $(OUTDIR)/$(OBJDIR)/*))
BADDEPS := $(filter-out $(DEPENDS),$(wildcard $(OUTDIR)/$(DEPDIR)/*))

# BUILD RULES
.PHONY: all clean

all: $(MAP) $(HEX) $(BIN) $(ASM)
	@rm -f $(BADOBJS) $(BADDEPS)
	arm-none-eabi-size $(OUT)

$(ASM): $(OUT)
	arm-none-eabi-objdump -dCSwz -marm -Mforce-thumb $< > $@

$(BIN): $(OUT)
	arm-none-eabi-objcopy -O binary $< $@

$(HEX): $(OUT)
	arm-none-eabi-objcopy -O ihex $< $@

%.out %.map: $(OBJECTS)
	arm-none-eabi-gcc -o $*.out -Wl,-Map=$*.map $(OBJECTS) $(LDFLAGS)

.SECONDARY: $(OBJECTS)

-include $(DEPENDS)

$(OUTDIR)/$(OBJDIR)/%.c.o: %.c $(OUTDIR)/$(DEPDIR)/%.c.d | $(OUTDIR)/$(OBJDIR) $(OUTDIR)/$(DEPDIR)
	arm-none-eabi-gcc -c -o $@ $< -MMD -MP -MF $(word 2,$^) $(CFLAGS)

$(OUTDIR)/$(OBJDIR)/%.S.o: %.S $(OUTDIR)/$(DEPDIR)/%.S.d | $(OUTDIR)/$(OBJDIR) $(OUTDIR)/$(DEPDIR)
	arm-none-eabi-gcc -x assembler-with-cpp -c -o $@ $< -MMD -MP -MF $(word 2,$^) $(ASMFLAGS)

$(DEPENDS):

$(OUTDIR)/$(OBJDIR) $(OUTDIR)/$(DEPDIR): | $(OUTDIR)
	mkdir $@

$(OUTDIR):
	mkdir $@

clean:
	rm -f -r $(OUTDIR)

flash: all
	nrfjprog --recover
	nrfjprog --program $(HEX) --sectorerase --verify
	nrfjprog --reset