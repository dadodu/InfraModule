################################################################################
#
# Makefile pour le module Infra - version 1.2
# 
# Librairie : stm32l1xx_stperiph_lib_v1.2.0 (22/02/13)
#
################################################################################

# Nom de l'executable
TARGET          = inframod

# Optimisation (0,1,2,s)
OPT             = s

# Adresse des repertoires
SRC_DIR         = ./src
INC_DIR         = ./inc
OBJ_DIR         = ./obj
BIN_DIR         = ./obj
RACINE          = ..
CMSIS_DIR       = $(RACINE)/lib/CMSIS
STDPERIPH_DIR   = $(RACINE)/lib/STM32L1xx_StdPeriph_Driver
INFRAMOD_DIR    = $(RACINE)/lib/InfraModule_Lib_v0.1

# Linker script
LINKER          = $(SRC_DIR)/startup/stm32_flash.ld

# Adresses des fichiers sources
VPATH          := $(SRC_DIR):$(SRC_DIR)/startup

# Adresses des fichiers sources : CMSIS
VPATH          := $(VPATH):$(CMSIS_DIR)/Device/ST/STM32L1xx/Source/Templates
CMSIS_SRC       = $(notdir $(wildcard $(CMSIS_DIR)/Device/ST/STM32L1xx/Source/Templates/*.c))

# Adresses des fichiers sources : STM32L1xx_StdPeriph_Driver
VPATH          := $(VPATH):$(STDPERIPH_DIR)/src
STDPERIPH_SRC   = $(notdir $(wildcard $(STDPERIPH_DIR)/src/*.c))

# Adresses des fichiers sources : InfraModule_Lib_v0.1
VPATH          := $(VPATH):$(INFRAMOD_DIR)/src
INFRAMOD_SRC    = $(notdir $(wildcard $(INFRAMOD_DIR)/src/*.c))

#-------------------------------------------------------------------------------
# Creation de la liste des fichiers sources
SOURCES = \
	$(notdir $(wildcard $(SRC_DIR)/startup/*.s)) \
	$(notdir $(wildcard $(SRC_DIR)/*.c)) \
	$(CMSIS_SRC) \
	$(STDPERIPH_SRC) \
	$(INFRAMOD_SRC)

INCLUDES = \
	$(INC_DIR) \
	$(CMSIS_DIR)/Device/ST/STM32L1xx/Include \
	$(CMSIS_DIR)/Include \
	$(STDPERIPH_DIR)/inc \
	$(INFRAMOD_DIR)/inc

ARCH_FLAGS = -mthumb -mcpu=cortex-m3

CFLAGS = \
	$(ARCH_FLAGS) \
	$(addprefix -I,$(INCLUDES)) \
	-O$(OPT) \
	-Wall \
	-ffunction-sections \
	-fdata-sections \
	-DSTM32L1XX_MD \
	-DUSE_STDPERIPH_DRIVER \
	-fno-exceptions \
	-Wno-write-strings

LDFLAGS = \
	$(ARCH_FLAGS) \
	-lm \
	-static \
	-Wl,-gc-sections \
	-T$(LINKER)

ASFLAGS = \
	$(ARCH_FLAGS) \
	-x assembler-with-cpp \
	$(addprefix -I,$(INCLUDES))

# Outils de compilation
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
CP      = arm-none-eabi-objcopy
OD      = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size


TARGET_BIN = $(BIN_DIR)/$(TARGET).bin
TARGET_HEX = $(OBJ_DIR)/$(TARGET).hex
TARGET_LST = $(OBJ_DIR)/$(TARGET).lst
TARGET_ELF = $(OBJ_DIR)/$(TARGET).elf
TARGET_OBJ = $(addsuffix .o,$(addprefix $(OBJ_DIR)/,$(basename $(SOURCES))))

#-------------------------------------------------------------------------------
all: version build run size

build: $(TARGET_BIN) $(TARGET_HEX) $(TARGET_LST)

$(TARGET_BIN): $(TARGET_ELF)
	@echo "-> "$(notdir $@)""
	@$(CP) -O binary $< $@

$(TARGET_HEX): $(TARGET_ELF)
	@echo "-> "$(notdir $@)""
	@$(CP) -O ihex $< $@

$(TARGET_LST): $(TARGET_ELF)
	@echo "-> "$(notdir $@)""
	@$(OD) -S $< > $@

$(TARGET_ELF): $(TARGET_OBJ)
	@echo -e "\033[01;33m==========  GENERATION  ====================================\033[m"
	@echo "-> "$(notdir $@)""
	@$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "\033[01;42m-> COMPILE:\033[m "$(notdir $<)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo -e "\033[01;42m-> COMPILE:\033[m "$(notdir $<)
	@$(CC) $(CFLAGS) -c -o $@ $<  

$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo -e "\033[01;42m-> COMPILE:\033[m "$(notdir $<)
	@$(CC) $(ASFLAGS) -c -o $@ $<

run: $(TARGET_BIN)
	@echo -e "\033[01;33m==========  PROGRAMMATION  =================================\033[m"
	@st-flash write $< 0x8000000

size: $(TARGET_ELF)
	@echo -e "\033[01;33m==========  TAILLE  ========================================\033[m"
	@$(SIZE) $<

clean:
	@echo -e "\033[01;41m-> SUPRESSION !\033[m"
	@rm -vf $(TARGET_BIN)
	@rm -vf $(TARGET_HEX)
	@rm -vf $(TARGET_LST)
	@rm -vf $(TARGET_ELF)
	@rm -vf $(SRC_DIR)/*~

purge:
	@echo -e "\033[01;41m-> SUPRESSION !!\033[m"
	@rm -vRf $(OBJ_DIR)

version:
ifeq ($(VERSION),USE_INFRAMODULE_V12)
	@echo -e "\033[01;41m /!\ ATTENTION : Compilation pour le module Infra version 1.2 /!\ \033[0m"
else ifeq ($(VERSION),USE_INFRAMODULE_V15)
	@echo -e "\033[01;41m /!\ ATTENTION : Compilation pour le module Infra version 1.5 /!\ \033[0m"
endif

help:
	@echo ""
	@echo "#==============================================================#"
	@echo "# Makefile pour STM32L-Discovery                               #"
	@echo "#                                                              #"
	@echo "# Usage: make [options]                                        #"
	@echo "#                                                              #"
	@echo "#  options:                                                    #"
	@echo "#   build     compiler le projet complet (avec *.o)            #"
	@echo "#   run       programmer le microcontroleur sur kit            #"
	@echo "#   size      afficher la taille du binaire                    #"
	@echo "#   clean     effacer les fichiers *.bin *.hex *.lst *.elf *~  #"
	@echo "#   purge     effacer le dossier obj/                          #"
	@echo "#   help      afficher l'aide memoire                          #"
	@echo "#==============================================================#"
	@echo ""

#-------------------------------------------------------------------------------
