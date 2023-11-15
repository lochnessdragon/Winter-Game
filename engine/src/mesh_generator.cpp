#include "mesh_generator.h"

#include <iostream>
#include <fstream>
#include <map>

#include "log.h"
#include <glm/glm.hpp>

#define PAR_SHAPES_T uint32_t
#define PAR_SHAPES_IMPLEMENTATION
#include <par_shapes.h>

float quadVertices[] = { -1.0f, -1.0f,
					  1.0f,  -1.0f,
					  1.0f,  1.0f,
					  -1.0f, 1.0f };

float quadUvs[] = { 0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f };

unsigned int quadIndices[] = { 0, 1, 2, 0, 2, 3 }; // ccw cull face test correct

std::shared_ptr<Mesh> createQuad() {
	return std::make_shared<Mesh>((GLuint)sizeof(quadIndices), quadIndices, (GLuint)sizeof(quadVertices), quadVertices, (GLuint)sizeof(quadUvs), quadUvs);
}

float cubeVertices[] = { -1.0f, 1.0f, 1.0f, // front top left		: 0
						 -1.0f, -1.0f, 1.0f, // front bottom left	: 1
						 1.0f, 1.0f, 1.0f, // front top right		: 2
						 1.0f, -1.0f, 1.0f, // front bottom right	: 3

						 1.0f, 1.0f, 1.0f, // front top right		: 4
						 1.0f, -1.0f, 1.0f, // front bottom right	: 5
						 1.0f, 1.0f, -1.0f, // back top right		: 6
						 1.0f, -1.0f, -1.0f, // back bottom right	: 7

						 1.0f, 1.0f, -1.0f, // back top right		: 8
						 1.0f, -1.0f, -1.0f, // back bottom right	: 9
						 -1.0f, 1.0f, -1.0f, // back top left		: 10
						 -1.0f, -1.0f, -1.0f, // back bottom left	: 11

						 -1.0f, 1.0f, -1.0f, // back top left		: 12
						 -1.0f, -1.0f, -1.0f, // back bottom left	: 13
						 -1.0f, 1.0f, 1.0f, // front top left		: 14
						 -1.0f, -1.0f, 1.0f, // front bottom left	: 15

						 -1.0f, 1.0f, -1.0f, // back top left		: 16
						 -1.0f, 1.0f, 1.0f, // front top left		: 17
						 1.0f, 1.0f, -1.0f, // back top right		: 18
						 1.0f, 1.0f, 1.0f, // front top right		: 19

						 -1.0f, -1.0f, -1.0f, // back bottom left	: 20
						 -1.0f, -1.0f, 1.0f, // front bottom left	: 21
						 1.0f, -1.0f, -1.0f, // back bottom right	: 22
						 1.0f, -1.0f, 1.0f, // front bottom right	: 23
					   };

float cubeUvs[] = { 0, 1, // tl
					0, 0, // bl
					1, 1, // tr
					1, 0, // br

					0, 1, // tl
					0, 0, // bl
					1, 1, // tr
					1, 0, // br

					0, 1, // tl
					0, 0, // bl
					1, 1, // tr
					1, 0, // br

					0, 1, // tl
					0, 0, // bl
					1, 1, // tr
					1, 0, // br

					0, 1, // tl
					0, 0, // bl
					1, 1, // tr
					1, 0, // br

					0, 1, // tl
					0, 0, // bl
					1, 1, // tr
					1, 0 // br
				  };

unsigned int cubeIndices[] = { 0, 1, 2, // front face, left triangle
					  2, 1, 3, // front face, right triangle

					  4, 5, 6, // right face, left triangle
					  5, 7, 6, // right face, right triangle

					  8, 9, 10, // back face, left triangle
					  9, 11, 10, // back face, right triangle

					  12, 13, 14, // left face, left triangle
					  13, 15, 14, // left face, right triangle
					  
					  16, 17, 18, // top face, left triangle
					  17, 19, 18, // top face, right triangle

					  20, 22, 21, // bottom face, left triangle
					  22, 23, 21 // bottom face, right triangle
					};

std::shared_ptr<Mesh> createCube() {
	return std::make_shared<Mesh>((GLuint)sizeof(cubeIndices), cubeIndices, (GLuint)sizeof(cubeVertices), cubeVertices, (GLuint)sizeof(cubeUvs), cubeUvs);
}

std::shared_ptr<Mesh> createUVSphere(int slices, int stacks) {
    par_shapes_mesh* meshData = par_shapes_create_parametric_sphere(slices, stacks);
    GLuint componentSize = meshData->npoints * sizeof(float);
    std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>((GLuint)(meshData->ntriangles * sizeof(unsigned int) * 3), meshData->triangles, componentSize * 3, meshData->points, componentSize * 2, meshData->tcoords);
	sphere->attachBuffer(GL_FLOAT, 3, componentSize * 3, meshData->normals);
    par_shapes_free_mesh(meshData);
    
    return sphere;
}

std::shared_ptr<Mesh> createIcoSphere(int subdivisions) {
    par_shapes_mesh* meshData = par_shapes_create_subdivided_sphere(subdivisions);
    GLuint componentSize = meshData->npoints * sizeof(float);
    std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>((GLuint) (meshData->ntriangles * sizeof(unsigned int) * 3), meshData->triangles, componentSize * 3, meshData->points, componentSize * 2, meshData->tcoords);
    par_shapes_free_mesh(meshData);
    
    return sphere;
}

/**
 * OBJ format is as follows:
 * line by line
 * # is comment
 * v is vertex. x y z [w] w defaults to 1
 * vt are texture coords u [v w]; v and w are optional and default to 0
 * vn are vertex normals in x y z might not be unit vectors
 * f are polygonal face elements in vertex_index/texture_index/normal_index indices start at one and can be negative (wrapping from the top of the list)
 */
// TODO: fix crashes when there are no vertices, uvs, normals, or indices
std::shared_ptr<Mesh> loadObj(const std::string& filename) {
    std::ifstream file(filename, std::ios::in);
	//file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    // allocate space to store temporary mesh data
    std::vector<float> temp_vertices;
    std::vector<float> temp_uvs;
    std::vector<float> temp_normals;
	
	// allocate space to store actual mesh data (by combining index information)
	std::map<std::string, unsigned int> vertexInfoToIndex; // map unique faces to one index
	unsigned int nextUnusedIndex = 0; // required for face assembly (see line.rfind("f", 0)
	std::vector<unsigned int> indices;
	std::vector<float> vertices;
	std::vector<float> uvs;
	std::vector<float> normals;
    
    if (file.is_open()) {
        std::string line;
        // read through the file, loading the mesh data
        while(std::getline(file, line)) {
            if (line.rfind("#", 0) == 0) {
                // comment
                continue;
            } else if (line.rfind("vt", 0) == 0) {
                // texture coordinate
                line.erase(0, line.find(" ") + 1);
                //Log::getRendererLog()->trace("Reading texture coordinate with info: {}", line);
                for (int i = 0; i < 2; i++) {
                    // extract u and v from line
                    size_t firstSpace = line.find(" ");
                    std::string num = line.substr(0, firstSpace);
                    temp_uvs.push_back(std::stof(num));
                    line.erase(0, firstSpace + 1);
                }
			} else if (line.rfind("vn", 0) == 0) {
				// normal
				line.erase(0, line.find(" ") + 1);
				//Log::getRendererLog()->trace("Reading normal with info: {}", line);
				for (int i = 0; i < 3; i++) {
					// extract the 3 normal components
					size_t nextSpace = line.find(" ");
					std::string num = line.substr(0, nextSpace);
					temp_normals.push_back(std::stof(num));
					line.erase(0, nextSpace + 1);
				}
			} else if (line.rfind("v", 0) == 0) { // have to search for v after vn and vt to avoid false positives.
				// vertex
				line.erase(0, line.find(" ") + 1);
				//Log::getRendererLog()->trace("Reading vertex with info: {}", line);
				for (int i = 0; i < 3; i++) {
					// extract x, y, and z from line
					size_t firstSpace = line.find(" ");
					std::string num = line.substr(0, firstSpace);
					temp_vertices.push_back(std::stof(num));
					line.erase(0, firstSpace + 1);
				}
			} else if (line.rfind("f", 0) == 0) {
                // faces
                // we will assume for now that faces do not contain slashes
                line.erase(0, line.find(" ") + 1);
                //Log::getRendererLog()->trace("Reading face with info: \"{}\"", line);
                for (int i = 0; i < 3; i++) {
                    // extract the first three set of indices from the line
                    size_t nextSpace = line.find(" ");
                    std::string vertexInfo = line.substr(0, nextSpace);
					//Log::getRendererLog()->trace("Extracted vertex: \"{}\"", vertexInfo);
					if(vertexInfoToIndex.count(vertexInfo) == 1) {
						// the data for this vertex has already been placed correctly, we can just add a new index to the indices buffer
						indices.push_back(vertexInfoToIndex[vertexInfo]);
					} else {
						// we need to extract the three indices
						// obj indices start at 1, but c++ and opengl expects them to start at 0
						vertexInfoToIndex[vertexInfo] = nextUnusedIndex;
						
						size_t nextSlash = vertexInfo.find("/");
						std::string vertexIdxStr = vertexInfo.substr(0, nextSlash);
						unsigned int vertexIdx = (unsigned int) (std::stoi(vertexIdxStr) - 1);
						vertexInfo.erase(0, nextSlash + 1);
						
						nextSlash = vertexInfo.find("/");
						std::string uvIdxStr = vertexInfo.substr(0, nextSlash);
						unsigned int uvIdx = (unsigned int) (std::stoi(uvIdxStr) - 1);
						vertexInfo.erase(0, nextSlash + 1);
						
						// the next part of the string should only contain the normal index
						unsigned int normalIdx = (unsigned int) (std::stoi(vertexInfo) - 1);
						
						//Log::getRendererLog()->trace("Found vertex_idx={}, uv_idx={}, normal_idx={} for face.", vertexIdx, uvIdx, normalIdx);
						
						for (unsigned int i = 0; i < 3; i++) {
							vertices.push_back(temp_vertices[(vertexIdx * 3) + i]);
						}
						
						for (unsigned int i = 0; i < 2; i++) {
							uvs.push_back(temp_uvs[(uvIdx * 2) + i]);
						}
						
						for (unsigned int i = 0; i < 3; i++) {
							normals.push_back(temp_normals[(normalIdx * 3) + i]);
						}
						
						indices.push_back(nextUnusedIndex);
						
						nextUnusedIndex++;
					}
					
					line.erase(0, nextSpace + 1);
                }
            }
        }
    } else {
        Log::getRendererLog()->error("Failed to read obj file: {}.", filename);
        throw std::runtime_error("Failed to read obj file!");
    }
	
    size_t vertexCount = temp_vertices.size() / 3;
    //uvs = std::vector<float>(vertexCount * 2, 0.0f); // hacky trick
    
	Log::getRendererLog()->trace("Loaded object: {} Vertices Count: {} UVs Count: {} Faces Count: {} Normals Count: {}", filename, vertices.size() / 3, uvs.size() / 2, indices.size() / 3, normals.size() / 3);
	
	// mesh creation function expects sizes in terms of bytes, not elements
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(indices.size() * sizeof(int), indices.data(), vertices.size() * sizeof(float), vertices.data(), uvs.size() * sizeof(float), uvs.data());
	
	if (normals.size() > 0) {
		mesh->attachBuffer(GL_FLOAT, 3, normals.size() * sizeof(float), normals.data());
	}
	
	return mesh;
}
