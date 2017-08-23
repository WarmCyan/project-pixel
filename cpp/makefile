# compiler 
CC=g++ -std=c++0x

# output name
TARGET=FlameFractalEngine

# directories
BINDIR=bin
OBJDIR=obj
SRCDIR=src
LIBDIR=lib

INC=-I$(LIB_DIR)/cpp -I$(LIBDIR)

SOURCES := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(LIBDIR)/*.cpp)
OBJECTS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

all: $(TARGET)
	@echo "Linking complete"
	
run: all
	@echo -e "\n$(C_GREEN_L)Running...$(C_RESET)\n----------------------------------------"
	@$(BINDIR)/$(TARGET)

re: clean all

rerun: clean run

debug: setdebug clean $(TARGET)
	@gdb $(BINDIR)/$(TARGET) -tui

setdebug:
	$(eval CC=g++ -ggdb)

# link
$(TARGET): $(OBJECTS)
	@echo "Compiling complete"
	@echo -e "$(C_MAGENTA)Linking into '$(C_MAGENTA_L)$(TARGET)$(C_MAGENTA)'...$(C_RESET)"
	@$(CC) -o $(BINDIR)/$(TARGET) $^

# compile
#$(OBJDIR)/%.o: $(SOURCES)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo -e "$(C_YELLOW)Compiling '$(C_YELLOW_L)$<$(C_YELLOW)'...$(C_RESET)"
	@$(CC) $(INC) -c -o $@ $<
	
$(OBJDIR)/%.o: $(LIBDIR)/%.cpp
	@echo -e "$(C_YELLOW)Compiling '$(C_YELLOW_L)$<$(C_YELLOW)'...$(C_RESET)"
	@$(CC) $(INC) -c -o $@ $<

clean:
	@echo -e "Cleaning..."
	@-rm $(OBJDIR)/*.o
	@-rm $(BINDIR)/$(TARGET)
	@echo -e "Cleaning complete"

.PHONY: clean all run re rerun debug setdebug
