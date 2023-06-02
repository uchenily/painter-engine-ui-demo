#CC=emcc
#LD="/opt/emscripten-llvm/bin/wasm-ld --max-memory=268435456"
#LDFLAGS="-Wl,--max-memory=268435456"
#LDFLAGS = \
#EMCC_CFLAGS = \
#COMMON_CFLAGS = \
#COMMON_LNFLAGS = \
#    --no-entry \
#    --strip-all \
#    --export-dynamic \
#    --initial-memory=131072 \
#    --max-memory=268435456 \
#    -error-limit=0 \
#    --lto-O3 \
#    -O3 \
#    --gc-sections

#-Wl,--max-memory=268435456 \

#emscripten makefile
#####################################################
target :=PainterEngine.html
project_path := ../../../fox-jump/project
platform_path := ../../../fox-jump/platform
painterengine_path := ../../../PainterEngine
#####################################################

project_build := $(wildcard $(project_path)/*.c)
project_build_o := $(patsubst %.c,%.o,$(project_build))

platform_build := $(wildcard $(platform_path)/webassembly/*.c)
platform_build_o := $(patsubst %.c,%.o,$(platform_build))

painterengine_build_core := $(wildcard $(painterengine_path)/core/*.c)
painterengine_build_painterengine_o := $(patsubst %.c,%.o,$(painterengine_build_core))

painterengine_build_kernel := $(wildcard $(painterengine_path)/kernel/*.c)
painterengine_build_painterengine_o += $(patsubst %.c,%.o,$(painterengine_build_kernel))

painterengine_build_architecture := $(wildcard $(painterengine_path)/architecture/*.c)
painterengine_build_painterengine_o += $(patsubst %.c,%.o,$(painterengine_build_architecture))

#painterengine_build_platform := $(wildcard $(platform_path)/webassembly/*.c)
#painterengine_build_painterengine_o += $(patsubst %.c,%.o,$(painterengine_build_platform))

#painterengine_build_platform := $(wildcard $(platform_path)/webassembly/*.cpp)
#painterengine_build_painterengine_o += $(patsubst %.cpp,%.o,$(painterengine_build_platform))

# nvim /usr/lib/emscripten/src/settings.js
#//var INITIAL_MEMORY = 16777216;
#var INITIAL_MEMORY = 16777216 * 16;
all:$(project_build_o) $(platform_build_o) $(painterengine_build_painterengine_o) 
	emcc $(project_build_o) $(platform_build_o) $(painterengine_build_painterengine_o) \
	-O3 \
	-o $(target) \
	-I "$(painterengine_path)" \
	-I "$(project_path)" \
	-I "$(platform_path)" \
	-I "$(platform_path)/webassembly" \
	-l websocket.js \
	-s EXPORTED_RUNTIME_METHODS="['cwrap','ccall']" \
	-s WEBSOCKET_SUBPROTOCOL='binary' \
	-s SOCKET_DEBUG=1 \
	-s INITIAL_MEMORY=268435456 \
	-s USE_SDL \
	--preload-file assets
	emrun --no_browser --port 8080 PainterEngine.html
	
	#-s ASSERTIONS \

$(project_path)/%.o:$(project_path)/%.c
	emcc -c $^ -o $@ -I "$(painterengine_path)" -I "$(platform_path)/webassembly"

$(platform_path)/webassembly/%.o:$(platform_path)/webassembly/%.c
	emcc -c $^ -o $@ -I "$(painterengine_path)" -I "$(project_path)" -I "$(platform_path)" -I "$(platform_path)/webassembly" -sUSE_SDL

$(painterengine_path)/architecture/%.o:$(painterengine_path)/architecture/%.c 
	emcc -c $^ -o $@ -I "$(painterengine_path)"

$(painterengine_path)/kernel/%.o:$(painterengine_path)/kernel/%.c
	emcc -c $^ -o $@

$(painterengine_path)/core/%.o:$(painterengine_path)/core/%.c
	emcc -c $^ -o $@

.PHONY:clean
clean:
	rm $(painterengine_path)/core/*.o
	rm $(painterengine_path)/kernel/*.o
	rm $(painterengine_path)/architecture/*.o
	rm $(platform_path)/webassembly/*.o
	rm $(project_path)/*.o
