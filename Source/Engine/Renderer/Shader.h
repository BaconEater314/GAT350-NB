#pragma once

namespace neu {
	class Shader : public Resource {
	public:
		~Shader();

		bool Load(const std::string& filename, GLuint shaderType);

	public:
		GLuint m_shader = 0;
	};
}