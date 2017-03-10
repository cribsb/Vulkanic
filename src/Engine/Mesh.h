#pragma once
#include "Helper.hpp"

class Mesh
{
public:
	Mesh(std::string filepath, float scale = 1.0f);
	~Mesh();

	std::vector<VE::Vertex> getVertices();
	std::vector<uint32_t> getIndices();

private:
	void loadModel(std::string filepath, float scale = 1.0f);

	std::vector<VE::Vertex> vertices;
	std::vector<uint32_t> indices;
};

