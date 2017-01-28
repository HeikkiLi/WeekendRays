#pragma once

#include "hitable.h"

class XYRect : public Hitable
{
public:
	XYRect() {}
	XYRect(float _x0, float _x1, float _y0, float _y1, float _k, Material *mat) :
		x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
	virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;


	Material *mp;
	float x0, x1, y0, y1, k;
};


bool XYRect::hit(const Ray & r, float t0, float t1, HitRecord & rec) const
{
	float t = (k - r.origin().z()) / r.direction().z();
	if (t < t0 || t > t1)
		return false;
	float x = r.origin().x() + t*r.direction().x();
	float y = r.origin().y() + t*r.direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.material = mp;
	rec.p = r.point(t);
	rec.normal = Vec3(0, 0, 1);

	return true;
}

inline bool XYRect::bounding_box(float t0, float t1, AABB & box) const
{
	return false;
}