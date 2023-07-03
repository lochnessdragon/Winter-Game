#include "mesh_generator.h"

float quadVertices[] = { -1.0f, -1.0f, 0.0f,
					  1.0f,  -1.0f, 0.0f,
					  1.0f,  1.0f,  0.0f,
					  -1.0f, 1.0f,  0.0f };

float quadUvs[] = { 0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f };

int quadIndices[] = { 0, 1, 2, 0, 2, 3 }; // ccw cull face test correct

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

int cubeIndices[] = { 0, 1, 2, // front face, left triangle
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