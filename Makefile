# ---------------------------------------
# Program Specific Defines
# ---------------------------------------
CC := g++
# This is necessary for skipping the file when compiling unit tests
MAIN_FILE_NAME := main.cpp
TARGET := main
TEST_TARGET := test_main
LIBRARIES := 
DEFINITIONS := 
FLAGS = -g -std=c++17 -Wall -Werror

# ---------------------------------------
# Probably don't need to change
# ---------------------------------------

# Directory layout
SDIR := source
ODIR := $(SDIR)/.obj
IDIRS := include #include/vendor
DDIR := $(ODIR)/deps
TDIR := $(SDIR)/test
TODIR := $(TDIR)/.obj
TDDIR := $(TODIR)/deps

# create flags
LIBS = $(patsubst %, -l%, $(LIBRARIES))
DEFS = $(patsubst %, -D%, $(DEFINITIONS))
IPATHS = $(patsubst %, -I%, $(IDIRS))
CFLAGS = $(FLAGS) $(IPATHS) $(LIBS) $(DEFS)

TEST_FLAGS = $(filter-out -lpthread, $(filter-out -g, $(CFLAGS))) -lgtest -lgtest_main -lpthread

# get source and object file names
SOURCE = $(notdir $(wildcard $(SDIR)/*.cpp))
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SOURCE))

TSOURCE = $(notdir $(wildcard $(TDIR)/*.cpp))
TOBJ = $(patsubst %.cpp, $(TODIR)/%.o, $(TSOURCE))

# compilation and linking
$(ODIR)/%.o : $(SDIR)/%.cpp
$(ODIR)/%.o : $(SDIR)/%.cpp $(DDIR)/%.d | $(DDIR)
	$(CC) -c -o $@ $< $(CFLAGS) -MT $@ -MMD -MP -MF $(DDIR)/$*.d

$(TODIR)/%.o : $(TDIR)/%.cpp
$(TODIR)/%.o : $(TDIR)/%.cpp $(TDDIR)/%.d | $(TDDIR)
	$(CC) -c -o $@ $< $(TEST_FLAGS) -MT $@ -MMD -MP -MF $(TDDIR)/$*.d

$(TARGET) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

test : $(filter-out $(patsubst %.cpp, $(ODIR)/%.o, $(MAIN_FILE_NAME)), $(OBJ)) $(TOBJ)
	$(CC) -o $(TEST_TARGET) $^ $(TEST_FLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(DDIR)/.d $(TARGET) $(TEST_TARGET) $(TODIR)/*.o $(TDDIR)/.d

setup:
	mkdir -p $(SDIR) $(IDIRS) $(ODIR) $(DDIR) $(TDIR) $(TODIR) $(TDDIR)
	@echo -e '$(TARGET)\n$(TEST_TARGET)\n$(ODIR)/*.o\n$(DDIR)/*.d\n$(TODIR)/*.o\n$(TDDIR)/*.d' > .gitignore
	@echo -e '#include <gtest/gtest.h>\n\nint main(int argc, char** argv)\n{\n\t::testing::InitGoogleTest(&argc, argv);\n\treturn RUN_ALL_TESTS();\n}' > $(TDIR)/main.cpp
	git init

$(DDIR): ; @mkdir -p $@

DEPFILES = $(patsubst %.cpp, $(DDIR)/%.d, $(SOURCE)) $(patsubst %.cpp, $(TDDIR)/%.d, $(TSOURCE))
$(DEPFILES):

include $(wildcard $(DEPFILES))
