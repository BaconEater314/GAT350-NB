#pragma once
#include "Resources/Resource.h"
#include "GUI/GUI.h"
#include <glm/glm.hpp>

namespace neu{
	class Shader;
	class Program;

	class Material : public Resource {
	public:
		enum class Parameters : uint32_t {
			None			= 0,
			BaseMap			= (1 << 0),
			SpecularMap		= (1 << 1),
			EmissiveMap		= (1 << 2),
			NormalMap		= (1 << 3),
			CubeMap			= (1 << 4),
			ShadowMap		= (1 << 5),
		};

		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();

		void UpdateGui() override;
	public:
		float shininess{ 2 };
		glm::vec2 tiling{ 1,1 };
		glm::vec2 offset{ 0,0 };

		res_t<Texture> baseMap;
		res_t<Program> program;
		glm::vec3 baseColor{ 1, 1, 1 };
		res_t<Texture> specularMap;
		res_t<Texture> emissiveMap;
		glm::vec3 emissiveColor{ 0, 0, 0 };
		res_t<Texture> normalMap;
		res_t<Texture> cubeMap;
		res_t<Texture> shadowMap;

		float ior{ 1.3f };

		Parameters parameters = Parameters::None;
	};
}