#pragma once
#include "Texture.h"

namespace neu {
	class CubeMap : public Texture, GUI {
	public:
		//CLASS_PROTOTYPE(CubeMap)

		bool Load(const std::string& filenames);

		void UpdateGui();
	};
}