NASM = nasm
NASM_FLAGS = -felf64
LIBASM = libasm.a

SRCS_DIR = src
ASM_DIR = $(SRCS_DIR)/asm
OBJS_DIR = $(SRCS_DIR)/.objs

MAIN_FILE = $(SRCS_DIR)/main.s
EXECUTABLE = program

C_MAIN_FILE = $(SRCS_DIR)/main.c
C_EXECUTABLE = cprogram

# ASM sources
SRCS = \
$(addprefix $(ASM_DIR)/ft_, \
$(addsuffix .s, \
	strlen strcpy strcmp write read strdup \
))

BONUS_SRCS = \
$(addprefix $(ASM_DIR)/ft_, \
$(addsuffix .s, \
	atoi_base \
))

# Detect debug flag
ifneq (,$(filter debug,$(MAKECMDGOALS)))
	NASM_FLAGS += -g
endif

# Detect bonus flag
ifneq (,$(filter bonus,$(MAKECMDGOALS)))
	SRCS += $(BONUS_SRCS)
endif

# Remove debug/bonus from goals so Make doesn't try to build them as targets
REAL_GOALS := $(filter-out debug bonus,$(MAKECMDGOALS))
ifeq ($(REAL_GOALS),)
	REAL_GOALS := all
endif

# Objects
OBJS := $(SRCS:$(ASM_DIR)/%.s=$(OBJS_DIR)/%.o)
BONUS_OBJS := $(BONUS_SRCS:$(ASM_BONUS_DIR)/%.s=$(OBJS_DIR)/bonus/%.o)

# Targets
all: $(LIBASM)

asm: $(EXECUTABLE)
c: $(C_EXECUTABLE)

# Main program
$(EXECUTABLE): $(MAIN_FILE) $(LIBASM)
	mkdir -p $(OBJS_DIR)
	$(NASM) $(NASM_FLAGS) $< -o $(OBJS_DIR)/$(EXECUTABLE).o
	ld $(OBJS_DIR)/$(EXECUTABLE).o $(LIBASM) -o $(EXECUTABLE)

# C program
$(C_EXECUTABLE): $(C_MAIN_FILE) $(LIBASM)
	cc $(C_MAIN_FILE) -L. -lasm -g3 -o $(C_EXECUTABLE)

# Library
$(LIBASM): $(OBJS)
	ar rcs $(LIBASM) $^

# Generic rules
$(OBJS_DIR)/%.o : $(ASM_DIR)/%.s
	@mkdir -p $(OBJS_DIR)
	@$(NASM) $(NASM_FLAGS) $< -o $@

$(OBJS_DIR)/bonus/%.o : $(ASM_BONUS_DIR)/%.s
	@mkdir -p $(OBJS_DIR)/bonus
	@$(NASM) $(NASM_FLAGS) $< -o $@

# Clean
clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(LIBASM) $(EXECUTABLE) $(C_EXECUTABLE)

re: fclean all

.PHONY: all clean fclean re debug bonus asm c
