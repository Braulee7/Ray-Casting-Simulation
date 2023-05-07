#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>
#include "Hittable.hpp"

class Scene : public Hittable {
public:
	Scene() : mLightDirection(glm::normalize(glm::vec3(-255, -255, -255))) { }
	Scene(std::shared_ptr<Hittable> obj) : mLightDirection(glm::normalize(glm::vec3(-255, -255, -255))) { add(obj); }
	Scene(glm::vec3& lightDirection) : mLightDirection(glm::normalize(glm::vec3(-255, -255, -255))) {}
	Scene(std::shared_ptr<Hittable> obj, glm::vec3& lightDirection) : mLightDirection(glm::normalize(lightDirection)) { add(obj); }
	void add(std::shared_ptr<Hittable> obj) { mObjects.push_back(obj); }
	virtual bool hit(const Ray& r, float min, float max, hitInfo& rec)const override;

public:
	glm::vec3 mLightDirection;

private:
	std::vector<std::shared_ptr<Hittable>> mObjects;
};

bool Scene::hit(const Ray& r, float min, float max, hitInfo& rec) const {
	hitInfo temp;
	bool hitAny = false;
	float closest = max;

	for (const auto& obj : mObjects) {
		if (obj->hit(r, min, closest, temp)) {
			hitAny = true;
			closest = temp.t;
			rec = temp;
		}
	}

	return hitAny;
}
#endif // !SCENE_HPP
