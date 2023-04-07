#include "HittableList.h"

HittableList::HittableList(std::shared_ptr<Hittable> object){
	add(object);
}

void HittableList::add(std::shared_ptr<Hittable> object) {
	mObjects.push_back(object);
}

bool HittableList::hit(const Ray& r, float tMin, float tMax, hitRecord& rec) const
{
	hitRecord temp;
	bool hitAny = false;
	float closest = tMax;

	for (const auto& object : mObjects) {
		if (object->hit(r, tMin, closest, temp)) {
			hitAny = true;
			closest = temp.t;
			rec = temp;
		}
	}

	return hitAny;
}
