#pragma once
#include "VEngine.hpp"
#include <map>

namespace VE
{
	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

		///////////////////////////////////////////////////////////////////////
		/// \fn	void ShaderManager::CreateProgram(const std::string shaderName, tr_renderer* renderer, tr_shader_program** shader_program);
		///
		/// \brief	Creates a program.
		///
		/// \author	Chris
		/// \date	24-2-2017
		///
		/// \param 		   	shaderName	  	Name of the shader.
		/// \param [in,out]	renderer	  	the renderer.
		/// \param [in,out]	shader_program	the shader program.
		///////////////////////////////////////////////////////////////////////

		void CreateProgram(const std::string shaderName, tr_renderer* renderer, tr_shader_program** shader_program);

		///////////////////////////////////////////////////////////////////////
		/// \fn	static const tr_shader_program* ShaderManager::getShader(const std::string& shaderName);
		///
		/// \brief	Gets a shader.
		///
		/// \author	Chris
		/// \date	24-2-2017
		///
		/// \param	shaderName	Name of the shader.
		///
		/// \return	Null if it fails, else the shader.
		///////////////////////////////////////////////////////////////////////

		static const tr_shader_program* getShader(const std::string& shaderName);

		///////////////////////////////////////////////////////////////////////
		/// \fn	static ShaderManager* ShaderManager::getShaderManager();
		///
		/// \brief	Gets shader manager.
		///
		/// \author	Chris
		/// \date	24-2-2017
		///
		/// \return	Null if it fails, else the shader manager.
		///////////////////////////////////////////////////////////////////////

		static ShaderManager* getShaderManager();
	private:

		/// \brief	The programs.
		static std::map<std::string, tr_shader_program> programs;

		///////////////////////////////////////////////////////////////////////
		/// \fn	std::vector<uint8_t> ShaderManager::LoadFile(const std::string& path);
		///
		/// \brief	Loads a file.
		///
		/// \author	Chris
		/// \date	24-2-2017
		///
		/// \param	path	Full pathname of the file.
		///
		/// \return	The contents of the file.
		///////////////////////////////////////////////////////////////////////

		std::vector<uint8_t> LoadFile(const std::string& path);

		/// \brief	The manager.
		static ShaderManager* manager;
	};
}
