#pragma once
#include "Resources/Resource.h"
#include <glad/glad.h>

namespace neu {
	class Shader : public Resource {
	public:
		~Shader();

		bool Load(const std::string& filename, GLuint shaderType);

	public:
		GLuint m_shader = 0;
	};
}