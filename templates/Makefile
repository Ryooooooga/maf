CARGO ?= cargo

.PHONY: gen
gen:
	${CARGO} run

.PHONY: test
test:
	${CARGO} test
	@${MAKE} lint

.PHONY: test-snap
test-snap:
	${CARGO} insta test --accept
	@${MAKE} lint

.PHONY: lint
lint:
	${CARGO} clippy --all-targets --all-features -- -D warnings

.PHONY: fmt
fmt:
	${CARGO} fmt
