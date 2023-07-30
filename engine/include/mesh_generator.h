#pragma once

/**
* mesh_generator.h:
*	contains procedures to provide meshes of varying standard quality with vertices, indices, and uv coordinates.
*	plans for model loading in here.
*/

#include <memory>
#include <string>

#include "mesh.h"

std::shared_ptr<Mesh> createQuad();
std::shared_ptr<Mesh> createCube();
std::shared_ptr<Mesh> createUVSphere(int slices, int stacks);
std::shared_ptr<Mesh> createIcoSphere(int subdivisions);

std::shared_ptr<Mesh> loadObj(const std::string& filename);
std::shared_ptr<Mesh> loadGLTF(const std::string& filename);
