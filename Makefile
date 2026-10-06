# Stack Overflow Workshop — build all challenge binaries.
# Flags are chosen PER CHALLENGE so each mitigation is observable and addresses
# stay stable during the session.

CC       := gcc
CXX      := g++
COMMON   := -g -no-pie -fno-pie -O0
NOCANARY := -fno-stack-protector
CANARY   := -fstack-protector-all
BIN      := bin

CHALLENGES := \
  $(BIN)/01_find_stack \
  $(BIN)/02_return_address \
  $(BIN)/03_backtrace \
  $(BIN)/04_hexdump \
  $(BIN)/05_ret2win \
  $(BIN)/06_canary \
  $(BIN)/07_leaf_O0 \
  $(BIN)/07_leaf_O2 \
  $(BIN)/08_cpp_exception

all: $(BIN) $(CHALLENGES) flag.txt
	@echo ""
	@echo "Built all challenges into ./$(BIN)/"
	@echo "Start with:  gdb -q ./bin/01_find_stack"

$(BIN):
	mkdir -p $(BIN)

flag.txt:
	@echo 'flag{you_control_the_instruction_pointer}' > flag.txt

# Observe-only challenges + ret2win: no canary, no PIE, with symbols.
$(BIN)/01_find_stack: challenges/01_find_stack.c | $(BIN)
	$(CC) $(COMMON) $(NOCANARY) $< -o $@
$(BIN)/02_return_address: challenges/02_return_address.c | $(BIN)
	$(CC) $(COMMON) $(NOCANARY) $< -o $@
$(BIN)/03_backtrace: challenges/03_backtrace.c | $(BIN)
	$(CC) $(COMMON) $(NOCANARY) $< -o $@
$(BIN)/04_hexdump: challenges/04_hexdump.c | $(BIN)
	$(CC) $(COMMON) $(NOCANARY) $< -o $@
$(BIN)/05_ret2win: challenges/05_ret2win.c | $(BIN)
	$(CC) $(COMMON) $(NOCANARY) $< -o $@

# Canary ON for this one.
$(BIN)/06_canary: challenges/06_canary.c | $(BIN)
	$(CC) $(COMMON) $(CANARY) $< -o $@

# Leaf: two optimization levels to compare the prologue.
$(BIN)/07_leaf_O0: challenges/07_leaf.c | $(BIN)
	$(CC) -g -no-pie -fno-pie -O0 $(NOCANARY) $< -o $@
$(BIN)/07_leaf_O2: challenges/07_leaf.c | $(BIN)
	$(CC) -g -no-pie -fno-pie -O2 $(NOCANARY) $< -o $@

# C++.
$(BIN)/08_cpp_exception: challenges/08_cpp_exception.cpp | $(BIN)
	$(CXX) $(COMMON) $(NOCANARY) $< -o $@

clean:
	rm -rf $(BIN) flag.txt

.PHONY: all clean
