#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <memory>
#include <vector>
#include "Sphere.h"
#include "Hittable.h"

class HittableList : Hittable {
public:
	HittableList(std::shared_ptr<Sphere> object);
	void add(std::shared_ptr<Sphere> object);
	virtual bool hit(const Ray& r, float tMin, float tMax, hitRecord& rec) const override;

private:
	std::vector<std::shared_ptr<Sphere>> mObjects;

};

#endif