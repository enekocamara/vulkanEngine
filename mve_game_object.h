#pragma once

#include "mve_model.h"


#include <memory>

namespace mve {

	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale{1.f, 1.f};
		float rotation;
		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix = {{c,s},{-s, c}};
			glm::mat2 scaleMat{{scale.x, .0f},{0.f,scale.y}};
			return rotMatrix * scaleMat;
		}
	};

class MveGameObject{
public:
	using id_t = unsigned int;
	static MveGameObject createGameObject(){
		static id_t currentId = 0;
		return MveGameObject{ currentId++ };
	}
	MveGameObject& operator=(const MveGameObject&) = delete;
	MveGameObject(const MveGameObject&) = delete;
	MveGameObject(MveGameObject&&) = default;
	MveGameObject& operator=(MveGameObject&&);

	id_t getId() { return id; }
	std::shared_ptr<MveModel>model{};
	glm::vec3 color{};
	Transform2dComponent transform2d;
private:
	MveGameObject(id_t objId) : id{ objId } {}
	id_t id;
};

}
