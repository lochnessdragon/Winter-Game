#pragma once

/**
* mesh_generator.h:
*	contains procedures to provide meshes of varying standard quality with vertices, indices, and uv coordinates.
*	plans for model loading in here.
*/

#include <memory>

#include "mesh.h"

std::shared_ptr<Mesh> createQuad();
std::shared_ptr<Mesh> createCube();