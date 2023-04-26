#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <memory>
#include <vector>
#include "Sphere.h"
#include "Hittable.h"

class HittableList : public Hittable {
public:
	HittableList() = default;
	HittableList(std::shared_ptr<Hittable> object);
	void add(std::shared_ptr<Hittable> object);
	virtual bool hit(const Ray& r, float tMin, float tMax, hitRecord& rec) const override;

private:
	std::vector<std::shared_ptr<Hittable>> mObjects;
};

#endif