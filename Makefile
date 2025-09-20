NASM = nasm -felf64

LIBASM = libasm.a

SRCS_DIR = src
ASM_DIR = $(SRCS_DIR)/asm
OBJS_DIR = $(SRCS_DIR)/.objs

MAIN_FILE = $(SRCS_DIR)/main.s
EXECUTABLE = program

C_MAIN_FILE = $(SRCS_DIR)/main.c
C_EXECUTABLE = cprogram

ASM_SRCS = $(addprefix $(ASM_DIR)/, \
	ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s \
	ft_read.s \
)

OBJS = $(ASM_SRCS:$(ASM_DIR)/%.s=$(OBJS_DIR)/%.o)

all: $(LIBASM)
asm: $(EXECUTABLE)
c: $(C_EXECUTABLE)

$(EXECUTABLE): $(MAIN_FILE) $(LIBASM)
	$(NASM) $(MAIN_FILE) -o $(OBJS_DIR)/$(EXECUTABLE).o
	ld $(OBJS_DIR)/$(EXECUTABLE).o $(LIBASM) -o $(EXECUTABLE)
	@rm $(EXECUTABLE)_tmp

$(C_EXECUTABLE): $(C_MAIN_FILE) $(LIBASM)
	cc $(C_MAIN_FILE) -L. -lasm -o $(C_EXECUTABLE)

$(LIBASM): $(OBJS)
	ar rcs $(LIBASM) $(OBJS)

$(OBJS_DIR)/%.o : $(ASM_DIR)/%.s
	mkdir -p $(OBJS_DIR)
	$(NASM) $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(LIBASM)
	rm -f $(EXECUTABLE)
	rm -f $(C_EXECUTABLE)

re: fclean all

.PHONY: all clean fclean re bonus