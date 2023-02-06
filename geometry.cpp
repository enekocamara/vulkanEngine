#include "geometry.h"

namespace mve {
	/*void Geometry::makeTriangle(std::vector<MveModel::Vertex>& vertices,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 up) {
		vertices.push_back({ { up },{ 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { right },{ 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { left },{ 1.0f, 1.0f, 1.0f} });
	}

	void Geometry::sierpinski(std::vector<MveModel::Vertex>& vertices,
		glm::vec2 left, glm::vec2 right, glm::vec2 up, int depth) {
		if (depth <= 0)
			makeTriangle(vertices, left, right, up);
		else {
			auto leftUp = 0.5f * (left + up);
			auto rightUp = 0.5f * (right + up);
			auto leftRight = 0.5f * (left + right);
			sierpinski(vertices, left, leftRight, leftUp, depth - 1);
			sierpinski(vertices, leftRight, right, rightUp, depth - 1);
			sierpinski(vertices, leftUp, rightUp, up, depth - 1);
		}
	}

	std::unique_ptr<MveModel> Geometry::createCircleModel(MveDevice& device, unsigned int numSides) {
		std::vector<MveModel::Vertex> uniqueVertices{};
		for (int i = 0; i < numSides; i++) {
			float angle = i * glm::two_pi<float>() / numSides;
			uniqueVertices.push_back({ {glm::cos(angle), glm::sin(angle)} });
		}
		uniqueVertices.push_back({});  // adds center vertex at 0, 0

		std::vector<MveModel::Vertex> vertices{};
		for (int i = 0; i < numSides; i++) {
			vertices.push_back(uniqueVertices[i]);
			vertices.push_back(uniqueVertices[(i + 1) % numSides]);
			vertices.push_back(uniqueVertices[numSides]);
		}
		return std::make_unique<MveModel>(device, vertices);
	}

	std::unique_ptr<MveModel> Geometry::createSquareModel(MveDevice& device, glm::vec2 offset){
		std::vector<MveModel::Vertex> vertices = {
		{{-0.5f, -0.5f}},
		{{0.5f, 0.5f}},
		{{-0.5f, 0.5f}},
		{{-0.5f, -0.5f}},
		{{0.5f, -0.5f}},
		{{0.5f, 0.5f}},
		};

		for (auto& v : vertices) {
			v.position += offset;
		}
		return std::make_unique<MveModel>(device, vertices);
	}

	std::unique_ptr<MveModel> Geometry::createTriangleModel(MveDevice& device, glm::vec2 left,
		glm::vec2 right, glm::vec2 up) {

		std::vector<MveModel::Vertex> vertices;
		makeTriangle(vertices, { 0.2f, 0.2f }, { -0.2f, 0.2f }, { 0.0f, -0.2f });
		return std::make_unique<MveModel>(device, vertices);

	 }*/
}