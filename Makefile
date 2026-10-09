CMAKE ?= cmake
CTEST ?= ctest
CLANG_FORMAT ?= clang-format
GIT ?= git

CC ?= cc
BUILD_TYPE ?= Debug

BUILD_DIR := build
TARGET_DIR := ${BUILD_DIR}/${CC}/${BUILD_TYPE}
FETCHCONTENT_BASE_DIR := ${BUILD_DIR}/_deps

export CMKR_CACHE := ${BUILD_DIR}/cmkr

.PHONY: build
build: gen
	${CMAKE} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DFETCHCONTENT_BASE_DIR=${FETCHCONTENT_BASE_DIR} -B ${TARGET_DIR}
	${CMAKE} --build ${TARGET_DIR} -j

.PHONY: run
run: build
	${TARGET_DIR}/maf ${ARGS}

.PHONY: debug
debug:
	@${MAKE} BUILD_TYPE=Debug build

.PHONY: release
release:
	@${MAKE} BUILD_TYPE=Release build

.PHONY: test
test: build
	${CTEST} -V --test-dir ${TARGET_DIR}
	@${MAKE} -C templates test

.PHONY: gen
gen:
	@${MAKE} -C templates gen

.PHONY: fmt
fmt:
	${GIT} ls-files -z "*.c" "*.h" | xargs -0 ${CLANG_FORMAT} -i
	@${MAKE} -C templates fmt

.PHONY: clean
clean:
	${RM} -r build
