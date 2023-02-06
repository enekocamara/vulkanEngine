#pragma once

#include "first_app.h"
//#include "simple_render_system.h"
//#include "mve_model.h"

namespace mve {
	class Geometry
	{
	public:
		static std::unique_ptr<MveModel> createSquareModel(MveDevice& device, glm::vec2 offset);
		static std::unique_ptr<MveModel> createCircleModel(MveDevice& device, unsigned int numSides);
		static std::unique_ptr<MveModel> createTriangleModel(MveDevice& device, glm::vec2 left,
			glm::vec2 right,
			glm::vec2 up);
		static void makeTriangle(std::vector<MveModel::Vertex>& vertices, glm::vec2 left,
			glm::vec2 right,
			glm::vec2 up);
		void sierpinski(std::vector<MveModel::Vertex>& vertices,
			glm::vec2 left,
			glm::vec2 right,
			glm::vec2 up,
			int depth);
	};
}

