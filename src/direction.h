#pragma once

#include <spdlog/fmt/ostr.h>

// Represents a direction
enum class Direction {
	None, // indicating a nuetral direction (might be useful, especially if the player isn't looking one way or another)
	Up,
	Down,
	Left,
	Right
};

template<>
struct fmt::formatter<Direction> : fmt::formatter<std::string>
{
	auto format(Direction direction, format_context& ctx) const -> decltype(ctx.out())
	{
		switch (direction) {
			case Direction::None:
				return format_to(ctx.out(), "Direction::None");
			case Direction::Up:
				return format_to(ctx.out(), "Direction::Up");
			case Direction::Down:
				return format_to(ctx.out(), "Direction::Down");
			case Direction::Left:
				return format_to(ctx.out(), "Direction::Left");
			case Direction::Right:
				return format_to(ctx.out(), "Direction::Right");
			default:
				return format_to(ctx.out(), "Unknown direction type");
		}
	}
};