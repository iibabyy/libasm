NASM = nasm -felf64

LIBASM = libasm.a

SRCS_DIR = src
ASM_DIR = $(SRCS_DIR)/asm
OBJS_DIR = $(SRCS_DIR)/.objs

MAIN_FILE = $(SRCS_DIR)/main.s
EXECUTABLE = program

CPP_MAIN_FILE = $(SRCS_DIR)/main.cpp
CPP_EXECUTABLE = cpp_program

ASM_SRCS = $(addprefix $(ASM_DIR)/, ft_strlen.s ft_strcpy.s)

OBJS = $(ASM_SRCS:$(ASM_DIR)/%.s=$(OBJS_DIR)/%.o)

all: $(LIBASM)
asm: $(EXECUTABLE)
cpp: $(CPP_EXECUTABLE)

$(EXECUTABLE): $(MAIN_FILE) $(LIBASM)
	$(NASM) $(MAIN_FILE) -o $(OBJS_DIR)/$(EXECUTABLE).o
	ld $(OBJS_DIR)/$(EXECUTABLE).o $(LIBASM) -o $(EXECUTABLE)
	@rm $(EXECUTABLE)_tmp

$(CPP_EXECUTABLE): $(CPP_MAIN_FILE) $(LIBASM)
	c++ $(CPP_MAIN_FILE) -L. -lasm -o $(CPP_EXECUTABLE)

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
	rm -f $(CPP_EXECUTABLE)

re: fclean all

.PHONY: all clean fclean re bonus