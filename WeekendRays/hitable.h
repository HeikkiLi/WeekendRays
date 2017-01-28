#pragma once

#include "ray.h"
#include "aabb.h"
#include "util.h"

class Material;


void get_sphere_uv(const Vec3& p, float& u, float& v)
{
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;

}

struct HitRecord
{
	float t;
	float u;
	float v;
	Vec3 p;
	Vec3 normal;
	Material *material;
};

class Hitable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;
};