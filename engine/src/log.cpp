#include <log.h>

#include <iostream>

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Log::rendererLog;
std::shared_ptr<spdlog::logger> Log::gameLog;

void Log::init() {
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	console_sink->set_pattern("%^[%T] [%n/%l]: %v%$");
	console_sink->set_level(spdlog::level::trace);
	
	spdlog::sinks_init_list sinks = { console_sink };

	rendererLog = createLogger("renderer", sinks);
	gameLog = createLogger("game", sinks);
}

std::shared_ptr<spdlog::logger> Log::createLogger(const std::string& name, spdlog::sinks_init_list sinks) {
	auto logger = std::make_shared<spdlog::logger>(name, sinks);
	logger->set_level(spdlog::level::trace);
	spdlog::register_logger(logger);

	return logger;
}