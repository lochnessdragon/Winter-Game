{ pkgs }: {
	deps = [
		pkgs.emscripten
        pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
        pkgs.cmake
        pkgs.python39
	];
}