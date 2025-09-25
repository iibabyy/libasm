NASM = nasm
NASM_FLAGS = -felf64 -g
LIBASM = libasm.a

SRCS_DIR = src
ASM_DIR = $(SRCS_DIR)/asm
OBJS_DIR = $(SRCS_DIR)/.objs

TEST_MAIN_FILES = $(SRCS_DIR)/main.c $(SRCS_DIR)/tests.c
TEST_EXCUTABLE = tests

# ASM sources
SRCS = \
$(addprefix $(ASM_DIR)/ft_, \
$(addsuffix .s, \
	strlen strcpy strcmp write read strdup \
))

BONUS_SRCS = \
$(addprefix $(ASM_DIR)/ft_, \
$(addsuffix .s, \
	atoi_base create_elem lst_add_front list_push_front list_size list_find list_remove list_sort list_remove_if \
))

ifneq (,$(filter test,$(MAKECMDGOALS)))
	SRCS += $(BONUS_SRCS)
else ifneq (,$(filter bonus,$(MAKECMDGOALS)))
	SRCS += $(BONUS_SRCS)
endif

# Objects
OBJS := $(SRCS:$(ASM_DIR)/%.s=$(OBJS_DIR)/%.o)

# Targets
all: $(LIBASM) $(SRCS)

bonus: all

test: $(LIBASM) $(TEST_EXCUTABLE)

# test program
$(TEST_EXCUTABLE): $(TEST_MAIN_FILES)
	@cc $(TEST_MAIN_FILES) -L. -lasm -g3 -o $(TEST_EXCUTABLE)
	@clear
	./$(TEST_EXCUTABLE)

# Library
$(LIBASM): $(OBJS)
	@ar rcs $(LIBASM) $^
	@echo libasm created !

# Generic rules
$(OBJS_DIR)/%.o : $(ASM_DIR)/%.s
	@mkdir -p $(OBJS_DIR)
	@$(NASM) $(NASM_FLAGS) $< -o $@

$(OBJS_DIR)/bonus/%.o : $(ASM_BONUS_DIR)/%.s
	@mkdir -p $(OBJS_DIR)/bonus
	@$(NASM) $(NASM_FLAGS) $< -o $@

# Clean
clean:
	@rm -rf $(OBJS_DIR)
	@echo cleaned !

fclean: clean
	@rm -f $(LIBASM) $(TEST_EXCUTABLE)

re: fclean all

.PHONY: all clean fclean re bonus test