CMD_OPEN = open
MAX_ERRORS = 10
ECHO_FLAG =

ifeq ($(OS), Windows_NT)	
	this_OS := Windows
else
	this_OS := $(shell uname -s)
endif

ifeq ($(this_OS), Linux)
	CMD_OPEN = xdg-open
	LIB_ADDITIONAL = -pthread
	ECHO_FLAG = -e
endif

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

CC=gcc

OBJ_DIR = obj
GCOV_OBJ_DIR = obj/gcov
TEST_OBJ_DIR = obj/tests
TEST_DIR = tests
MODULES_DIR = matrix
REPORT_DIR = report

TEST_EXE = test
GCOV_EXE = gcov_report

MODULES_STATIC_LIBRARY = s21_matrix.a
MODULES_STATIC_LIBRARY_GCOV = s21_matrix_gcov.a

MODE?=1
ifeq ($(MODE), 2)
	LFLAGS=-g -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover \
	-fstack-protector -fanalyzer
	CFLAGS_TESTS=-Wall -Werror -Wextra -c -g -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable \
	-fno-sanitize-recover -fstack-protector
	CFLAGS += $(CFLAGS_TESTS) -fanalyzer
else 
	ifeq ($(MODE), 3)
		LFLAGS=-g
		CFLAGS=-Wall -Werror -Wextra -c -g
		CFLAGS_TESTS = $(CFLAGS)
	else 
		ifeq ($(MODE), 4)
			LFLAGS=-g -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector -fanalyzer
			CFLAGS=-Wall -Werror -Wextra -c -g -O2 -pedantic -std=c11 -Wshadow -Wlogical-op -Wshift-overflow=2 \
			-Wduplicated-cond -Wcast-align -Wfloat-equal -Wconversion -Wcast-qual -D_GLIBCXX_DEBUG \
			-D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined \
			-fno-sanitize-recover -fstack-protector -fanalyzer
			CFLAGS_TESTS = $(CFLAGS)
		else
			LFLAGS= 
			CFLAGS=-Wall -Werror -Wextra -c -std=c11 -pedantic
			CFLAGS_TESTS = $(CFLAGS)
		endif
	endif
endif

GCOV_FLAGS = -fprofile-arcs -ftest-coverage

LIBS = -lcheck $(LIB_ADDITIONAL)

LIBS_GCOV = $(LIBS) -lgcov

ALL_MODULES_DIRS = $(shell find $(MODULES_DIR) -type d)
ALL_MODULES_C = $(notdir $(shell find $(ALL_MODULES_DIRS) -maxdepth 1 -name "*.c"))
ALL_MODULES_H = $(shell find $(ALL_MODULES_DIRS) -maxdepth 1 -name "*.h")
MAIN_H = $(shell find . -maxdepth 1 -name "*.h")
ALL_H = $(ALL_MODULES_H) $(MAIN_H)
ALL_MODULES_O = $(ALL_MODULES_C:%.c=%.o)

ALL_TESTS_DIRS = $(shell find $(TEST_DIR) -type d)
ALL_TESTS_C = $(notdir $(shell find $(ALL_TESTS_DIRS) -maxdepth 1 -name "*.c"))
ALL_TESTS_H = $(shell find $(ALL_TESTS_DIRS) -maxdepth 1 -name "*.h")
ALL_TESTS_O = $(addprefix $(TEST_OBJ_DIR)/, $(notdir $(ALL_TESTS_C:%.c=%.o)))

ALL_MODULES_GCOV_O = $(addprefix $(GCOV_OBJ_DIR)/, $(notdir $(ALL_MODULES_C:%.c=%.o)))

ALL_CPP_H_FILES = $(shell find , -type f | grep "\.(c|h)$$" -E)

vpath %.cc $(ALL_MODULES_DIRS) : $(ALL_TESTS_DIRS)
vpath %.o $(OBJ_DIR)

all: $(TEST_EXE) $(GCOV_EXE)
main: 
	@echo "$(ALL_TESTS_O)"

$(TEST_EXE) : $(OBJ_DIR) $(TEST_OBJ_DIR) $(ALL_TESTS_O) $(MODULES_STATIC_LIBRARY)
	@$(CC) $(LFLAGS) $(ALL_TESTS_O) $(MODULES_STATIC_LIBRARY) $(LIBS) -o $(TEST_EXE)
	@./$(TEST_EXE)

$(GCOV_EXE) : $(GCOV_OBJ_DIR) $(TEST_OBJ_DIR) $(REPORT_DIR) $(ALL_TESTS_O) $(MODULES_STATIC_LIBRARY_GCOV)
	@$(CC) $(LFLAGS) $(ALL_TESTS_O) $(MODULES_STATIC_LIBRARY_GCOV) $(LIBS_GCOV) -o $(GCOV_EXE)
	@./$(GCOV_EXE)
	@gcov -o $(GCOV_OBJ_DIR) $(ALL_MODULES_C)
	@lcov -b ./ -d $(GCOV_OBJ_DIR) --gcov-tool /usr/bin/gcov -c -o output.info --no-external
	@genhtml -o $(REPORT_DIR) output.info
	@rm *.gcov output.info
	@$(CMD_OPEN) $(REPORT_DIR)/index.html

$(MODULES_STATIC_LIBRARY) : $(OBJ_DIR) $(TEST_OBJ_DIR) $(ALL_MODULES_O) $(ALL_H)
	@echo $(ECHO_FLAG) "$(GREEN)*$(RESET)\c"
	@ar rc $(MODULES_STATIC_LIBRARY) $(addprefix $(OBJ_DIR)/, $(ALL_MODULES_O))
	@ranlib $(MODULES_STATIC_LIBRARY)

$(MODULES_STATIC_LIBRARY_GCOV) : $(GCOV_OBJ_DIR) $(TEST_OBJ_DIR) $(ALL_MODULES_GCOV_O) $(ALL_H)
	@echo $(ECHO_FLAG) "$(GREEN)*$(RESET)\c"
	@ar rc $(MODULES_STATIC_LIBRARY_GCOV) $(ALL_MODULES_GCOV_O)
	@ranlib $(MODULES_STATIC_LIBRARY_GCOV)

%.o: $(MODULES_DIR)/%.c $(ALL_H) 
	@echo $(ECHO_FLAG) "$(GREEN)*$(RESET)\c"
	@$(CC) $(CFLAGS) -o $(addprefix $(OBJ_DIR)/, $@) $<

$(GCOV_OBJ_DIR)/%.o: $(MODULES_DIR)/%.c $(ALL_H) $(ALL_TESTS_H)
	@echo $(ECHO_FLAG) "$(GREEN)*$(RESET)\c"
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $@ $<

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(ALL_H) $(ALL_TESTS_H)
	@echo $(ECHO_FLAG) "$(GREEN)*$(RESET)\c"
	@$(CC) $(CFLAGS_TESTS) -o $@ $<

$(REPORT_DIR):
	@mkdir -p $(REPORT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(GCOV_OBJ_DIR):
	@mkdir -p $(GCOV_OBJ_DIR)

$(TEST_OBJ_DIR):
	@mkdir -p $(TEST_OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(REPORT_DIR) 
	rm -f *.a *.o ./$(GCOV_EXE) ./$(TEST_EXE)
	rm -f *.gcov *.gcno *.gcda output.info

rebuild: clean all

cpplint:
	clang-format -n --verbose $(ALL_CPP_H_FILES)

cppcheck: 
	cppcheck --enable-all --force --suppress=missingIncludeSystem --language=c++ $(ALL_CPP_H_FILES)

valgrind:
	valgrind --tool=memcheck --track-fds=yes --trace-children=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./$(TEST_EXE)

.PHONY: all clean rebuild cpplint cppcheck valgrind