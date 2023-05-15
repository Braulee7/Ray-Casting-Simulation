#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "Triangle.hpp"

class Mesh : public Hittable {
public:
	Mesh() = default;
	Mesh(glm::vec3& a, glm::vec3& b, glm::vec3& c) {
		glm::vec3 col(255);
		Add(a, b, c, col , 0);
	}
	Mesh(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& col, float smoothness = 0.0f) {
		Add(a, b, c, col, smoothness);
	}

	Mesh(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 na, glm::vec3 nb, glm::vec3 nc, glm::vec3 col, float smoothness = 0) {
		Add(a, b, c, na, nb, nc, col, smoothness);
	}

	void Add(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 na, glm::vec3 nb, glm::vec3 nc, glm::vec3 col, float smoothness = 0) {
		triangles.emplace_back(a, b, c, na, nb, nc, col, smoothness);
	}


	void Add(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& col, float smoothness = 0.0f) {
		triangles.emplace_back(a, b, c, col, smoothness);
	}

	virtual bool hit(const Ray& r, float min, float max, hitInfo& rec) const override;

private:
	std::vector<Triangle> triangles;

};

bool Mesh::hit(const Ray& r, float min, float max, hitInfo& rec) const {
	hitInfo temp;
	bool hitAny = false;
	float closest = max;

	for (const auto& obj : triangles) {
		if (obj.hit(r, min, closest, temp)) {
			hitAny = true;
			closest = temp.t;
			rec = temp;
		}
	}

	return hitAny;
}
#endif // !MESH_HPP
