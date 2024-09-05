EXE=fetchmail
SRC_DIR=fetchmail_program/src

# All Rust source files
SRC_FILES = \
	$(SRC_DIR)/main.rs \
	$(SRC_DIR)/communication.rs \
	$(SRC_DIR)/command.rs \
	$(SRC_DIR)/utils.rs

# Default target to build the executable
$(EXE): $(SRC_FILES) vendor
	cargo build --frozen --offline --release --manifest-path fetchmail_program/Cargo.toml
	cp fetchmail_program/target/release/$(EXE) .  # Copy the executable to the current directory

vendor:
	if [ ! -d "fetchmail_program/vendor/" ]; then \
		cd fetchmail_program && cargo vendor --locked; \
	fi

clean:
	cd fetchmail_program && cargo clean
	rm -rf fetchmail_program/vendor
	rm -f $(EXE)

.PHONY: clean vendor