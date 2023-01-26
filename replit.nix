{ pkgs }: {
	deps = [
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
        pkgs.cmake
        pkgs.glfw
        pkgs.glm
        pkgs.entt
        pkgs.spdlog
        pkgs.fmt_9
        #pkgs.emscripten
	];
}