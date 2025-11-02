emcc wasm_test.cpp -o tc.js \
  -std=c++20 \
  -I$HOME/emscripten-libs/wasm/include/base \
  -I$HOME/emscripten-libs/wasm/include/pipeline \
  -L$HOME/emscripten-libs/wasm/lib \
  -ltab_converter \
  -s EXPORTED_FUNCTIONS='["_process_data"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
  -s MODULARIZE=1
