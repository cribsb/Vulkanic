#include "VEShaderManager.h"

namespace VE
{
	std::map<std::string, tr_shader_program> ShaderManager::programs;
	ShaderManager* ShaderManager::manager;

	ShaderManager::ShaderManager()
	{
		
	}

	ShaderManager::~ShaderManager()
	{
		programs.clear();
	}


	void ShaderManager::CreateProgram(const std::string shaderName, tr_renderer* renderer, tr_shader_program** shader_program)
	{
		//TODO optimize door te checken of shader al bestaat
		std::string vertName = shaderName + "_vert.spv";
		std::string fragName = shaderName + "_frag.spv";
		auto vert = LoadFile(vertName);
		auto frag = LoadFile(fragName);
		tr_create_shader_program(VEngine::getEngine()->getRenderer(),
								 vert.size(), reinterpret_cast<uint32_t*>(vert.data()), "main",
								 frag.size(), reinterpret_cast<uint32_t*>(frag.data()), "main", shader_program);
	}

	const tr_shader_program* ShaderManager::getShader(const std::string& shaderName)
	{
		return &programs.at(shaderName);
	}

	ShaderManager* ShaderManager::getShaderManager()
	{
		if(manager == nullptr)
		{
			manager = new ShaderManager();
		}
		return manager;
	}


	std::vector<uint8_t> ShaderManager::LoadFile(const std::string& path)
	{
		std::ifstream is;
		std::string newpath = "assets/" + path;
		is.open(newpath.c_str(), std::ios::in | std::ios::binary);
		assert(is.is_open());

		is.seekg(0, std::ios::end);
		std::vector<uint8_t> buffer(is.tellg());
		assert(0 != buffer.size());

		is.seekg(0, std::ios::beg);
		is.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

		return buffer;
	}
}
