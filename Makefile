CMAKE ?= cmake
CTEST ?= ctest
CLANG_FORMAT ?= clang-format
GIT ?= git

CC ?= cc
BUILD_TYPE ?= Debug

BUILD_DIR := build
TARGET_DIR := ${BUILD_DIR}/${CC}/${BUILD_TYPE}

export CMKR_CACHE := ${BUILD_DIR}/cmkr

.PHONY: build
build:
	${CMAKE} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -B ${TARGET_DIR}
	${CMAKE} --build ${TARGET_DIR} -j

.PHONY: debug
debug:
	@${MAKE} BUILD_TYPE=Debug build

.PHONY: release
release:
	@${MAKE} BUILD_TYPE=Release build

.PHONY: test
test: build
	${CTEST} -V --test-dir ${TARGET_DIR}

.PHONY: fmt
fmt:
	${GIT} ls-files -z "*.c" "*.h" | xargs -0 ${CLANG_FORMAT} -i

.PHONY: clean
clean:
	${RM} -r build
