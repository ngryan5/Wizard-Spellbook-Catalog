# Compiler
CC = g++

# Executable file name
exe_file = wizard_catalog

# Object files
OBJ = catalog.o run_wizard.o

# Default target
$(exe_file): $(OBJ)
	$(CC) $(OBJ) -o $(exe_file)

# Rule to compile catalog.cpp into catalog.o
catalog.o: catalog.cpp
	$(CC) -c catalog.cpp

# Rule to compile run_wizard.cpp into run_wizard.o
run_wizard.o: run_wizard.cpp
	$(CC) -c run_wizard.cpp

# Clean target to remove object files and the executable
clean:
	rm -f *.o $(exe_file)

