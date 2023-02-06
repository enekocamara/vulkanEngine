#pragma once
#include "first_app.h"

#include <vector>
namespace mve {
	class Gravity
	{
	public:
		static void createField(std::vector<MveModel::Vertex>& vertices);
		static void changeDirection(std::vector<MveModel::Vertex>& vertices);
	};

}