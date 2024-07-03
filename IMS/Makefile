# Author: Petr Kaška <xkaska01>
# Author: Martin Hemza <xhemza05>

EXECUTABLE := ims_projekt
BUILD_DIR := cmake-build-release
DOC_DIR := doc
SRC_DIR := src


.PHONY: build
build: $(EXECUTABLE)

.PHONY: $(EXECUTABLE)
$(EXECUTABLE):
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Release ..
	cmake --build $(BUILD_DIR)


.PHONY: run
run:
	./$(BUILD_DIR)/$(EXECUTABLE) $(ARGS)


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)