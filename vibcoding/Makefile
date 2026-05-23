CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -D_GNU_SOURCE
TARGET = atm
TEST_TARGET = test_runner

SOURCES = main.cpp card.cpp account.cpp transaction.cpp journal.cpp recovery.cpp atm_engine.cpp file_storage.cpp
OBJECTS = $(SOURCES:.cpp=.o)

TEST_SOURCES = tests/test_runner.cpp card.cpp account.cpp transaction.cpp journal.cpp atm_engine.cpp file_storage.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "✅ Компиляция завершена! Запустите ./atm"

tests: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS)
	@echo "✅ Тесты скомпилированы! Запустите ./$(TEST_TARGET)"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGET)
	rm -rf data/*.bin data/backup_*

run: $(TARGET)
	./$(TARGET)

run-tests: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean run run-tests tests