CARGO ?= cargo

.PHONY: gen
gen:
	${CARGO} run

.PHONY: test
test:
	${CARGO} test
	${CARGO} clippy --all-targets --all-features -- -D warnings

.PHONY: fmt
fmt:
	${CARGO} fmt
