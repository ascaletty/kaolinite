CC= clang
CFLAGS= -std=c99 -Wall -Os -DCLAY_WASM -mbulk-memory --target=wasm32 -nostdlib -Wl,--strip-all -Wl,--export-dynamic -Wl,--no-entry -Wl,--export=__heap_base -Wl,--initial-memory=6553600 -o build/clay/index.wasm -lm
terminal: terminal.c
	$(CC) $(CFLAGS) terminal.c 
