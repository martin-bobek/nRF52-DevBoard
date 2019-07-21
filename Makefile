# PROJECT CONFIGURATION
OPT = -O3 -flto

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

LDDESC = config.ld

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

WARNINGS = \
	-Wall \
	-Wextra \
	-Wfloat-equal \
	-Wcast-align \
	-Wwrite-strings \
	-Wlogical-op \
	-Wmissing-declarations \
	-Wredundant-decls \
	-Wshadow

MACHINE = \
	-mcpu=cortex-M4 \
	-mthumb \
	-mabi=aapcs \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16

ASMFLAGS := \
	$(MACHINE) \
	$(DEBUG) \
	$(DEFINE) \
	$(INCDIR)

CSTD = -std=c99
CPPSTD = -std=c++14

CFLAGS := \
	-ffunction-sections \
	-fdata-sections \
	-fno-strict-aliasing \
	-fno-builtin \
	-fshort-enums \
	$(WARNINGS) \
	$(MACHINE) \
	$(OPT) \
	$(DEBUG) \
	$(DEFINE) \
	$(INCDIR)

LDFLAGS := \
	-Wl,--gc-sections \
	--specs=nano.specs \
	$(MACHINE) \
	$(OPT) \
	$(DEBUG) \
	$(ARCHIVE) \
	-L$(N5_MDK) \
	-T$(LDDESC)

# BUILD PREPARATION
MAKEFILE := $(word 1,$(MAKEFILE_LIST))

ASM := $(OUTDIR)/$(TARGET).asm
HEX := $(OUTDIR)/$(TARGET).hex
BIN := $(OUTDIR)/$(TARGET).bin
MAP := $(OUTDIR)/$(TARGET).map
OUT := $(OUTDIR)/$(TARGET).out

EXTS = .c .cpp .S
TARGETS := $(ASM) $(HEX) $(BIN) $(MAP) $(OUT) $(addprefix $(OUTDIR)/,$(OBJDIR) $(DEPDIR))
SOURCES := $(foreach DIR,$(SRCDIR),$(subst $(DIR)/,,$(wildcard $(addprefix $(DIR)/*,$(EXTS)))))
OBJECTS := $(addprefix $(OUTDIR)/$(OBJDIR)/,$(addsuffix .o,$(LIB_SRC) $(SOURCES)))
DEPENDS := $(addprefix $(OUTDIR)/$(DEPDIR)/,$(addsuffix .d,$(LIB_SRC) $(SOURCES)))

BADTRGS := $(filter-out $(TARGETS),$(wildcard $(OUTDIR)/*))
BADOBJS := $(filter-out $(OBJECTS),$(wildcard $(OUTDIR)/$(OBJDIR)/*))
BADDEPS := $(filter-out $(DEPENDS),$(wildcard $(OUTDIR)/$(DEPDIR)/*))

# BUILD RULES
.PHONY: all clean

all: $(MAP) $(HEX) $(BIN) $(ASM)
	@rm -fr $(BADTRGS) $(BADOBJS) $(BADDEPS)
	arm-none-eabi-size $(OUT)

$(ASM): $(OUT)
	arm-none-eabi-objdump -dCSwz -marm -Mforce-thumb $< > $@

$(BIN): $(OUT)
	arm-none-eabi-objcopy -O binary $< $@

$(HEX): $(OUT)
	arm-none-eabi-objcopy -O ihex $< $@

%.out %.map: $(OBJECTS) $(LDDESC)
	arm-none-eabi-g++ -o $*.out -Wl,-Map=$*.map $(OBJECTS) $(LDFLAGS)

.SECONDARY: $(OBJECTS)

-include $(DEPENDS)

$(OUTDIR)/$(OBJDIR)/%.c.o: %.c $(OUTDIR)/$(DEPDIR)/%.c.d $(MAKEFILE) | $(OUTDIR)/$(OBJDIR) $(OUTDIR)/$(DEPDIR)
	arm-none-eabi-gcc -c -o $@ $< -MMD -MP -MF $(word 2,$^) $(CSTD) $(CFLAGS)

$(OUTDIR)/$(OBJDIR)/%.cpp.o: %.cpp $(OUTDIR)/$(DEPDIR)/%.cpp.d $(MAKEFILE) | $(OUTDIR)/$(OBJDIR) $(OUTDIR)/$(DEPDIR)
	arm-none-eabi-g++ -c -o $@ $< -MMD -MP -MF $(word 2,$^) $(CPPSTD) $(CFLAGS)

$(OUTDIR)/$(OBJDIR)/%.S.o: %.S $(OUTDIR)/$(DEPDIR)/%.S.d $(MAKEFILE) | $(OUTDIR)/$(OBJDIR) $(OUTDIR)/$(DEPDIR)
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
	nrfjprog --program $(HEX) --verify --reset
