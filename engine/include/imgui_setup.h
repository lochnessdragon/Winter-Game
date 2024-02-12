#pragma once

// System files
#include <memory>

// Engine - dependency files
#include <imgui.h>

// Engine files
#include "window.h"

void setupImgui(std::shared_ptr<Window> win);
void shutdownImgui();

void imguiStartFrame();
void imguiEndFrame();