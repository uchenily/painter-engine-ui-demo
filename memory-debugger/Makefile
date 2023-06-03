all:
	meson compile -C build
setup:
	meson setup --reconfigure build --cross-file meson-cross.ini
	cp platform/webassembly/assets -r build
