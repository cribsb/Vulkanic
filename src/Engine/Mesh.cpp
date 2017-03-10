#include "Mesh.h"
#include <unordered_map>
#include "tiny_obj_loader.h"


Mesh::Mesh(std::string filepath, float scale)
{
	loadModel(filepath, scale);
}

Mesh::~Mesh()
{
}

std::vector<VE::Vertex> Mesh::getVertices()
{
	return vertices;
}

std::vector<uint32_t> Mesh::getIndices()
{
	return indices;
}


void Mesh::loadModel(std::string filepath, float scale)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str()))
	{
		//TODO make proper error stuff
		return;
	}

	std::unordered_map<VE::Vertex, int> uniqueVertices = {};
	for(const auto& shape: shapes)
	{
		for(const auto& index : shape.mesh.indices)
		{
			VE::Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2],
				1.0f/scale
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			//vertex.color = {1.0f, 1.0f, 1.0f};

			if(uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = vertices.size();
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
