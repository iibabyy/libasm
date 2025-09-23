NASM = nasm
NASM_FLAGS = -felf64
LIBASM = libasm.a

SRCS_DIR = src
ASM_DIR = $(SRCS_DIR)/asm
OBJS_DIR = $(SRCS_DIR)/.objs

TEST_MAIN_FILES = $(SRCS_DIR)/main.c $(SRCS_DIR)/tests.c
TEST_EXCUTABLE = test

# ASM sources
SRCS = \
$(addprefix $(ASM_DIR)/ft_, \
$(addsuffix .s, \
	strlen strcpy strcmp write read strdup \
))

BONUS_SRCS = \
$(addprefix $(ASM_DIR)/ft_, \
$(addsuffix .s, \
	atoi_base create_elem lst_add_front list_push_front list_size \
))

# Detect debug flag
ifneq (,$(filter debug,$(MAKECMDGOALS)))
	NASM_FLAGS += -g
endif

# Detect bonus flag
ifneq (,$(filter bonus,$(MAKECMDGOALS)))
	SRCS += $(BONUS_SRCS)
else ifneq (,$(filter test,$(MAKECMDGOALS)))
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

test: $(TEST_EXCUTABLE)

# test program
$(TEST_EXCUTABLE): $(TEST_MAIN_FILES) $(LIBASM)
	cc $(TEST_MAIN_FILES) -L. -lasm -g3 -o $(TEST_EXCUTABLE)
	@clear
	./$(TEST_EXCUTABLE)

# Library
$(LIBASM): $(OBJS)
	ar rcs $(LIBASM) $^

# Generic rules
$(OBJS_DIR)/%.o : $(ASM_DIR)/%.s
	@mkdir -p $(OBJS_DIR)
	$(NASM) $(NASM_FLAGS) $< -o $@

$(OBJS_DIR)/bonus/%.o : $(ASM_BONUS_DIR)/%.s
	@mkdir -p $(OBJS_DIR)/bonus
	$(NASM) $(NASM_FLAGS) $< -o $@

# Clean
clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(LIBASM) $(TEST_EXCUTABLE)

re: fclean all

debug:
	@echo -n
bonus:
	@echo -n

.PHONY: all clean fclean re asm