compile:
	meson compile -C build

recompile:
	meson setup --reconfigure build
	meson compile -C build

