#pragma once

#include <string>

#include <spdlog/spdlog.h>

class Log {
private:
	static std::shared_ptr<spdlog::logger> gameLog;
	static std::shared_ptr<spdlog::logger> rendererLog;
public:
	static void init();
	static std::shared_ptr<spdlog::logger> createLogger(const std::string& name, spdlog::sinks_init_list list);

	static std::shared_ptr<spdlog::logger> getGameLog() { return gameLog; }
	static std::shared_ptr<spdlog::logger> getRendererLog() { return rendererLog; }
};