#pragma once


#include "hitable.h"

class MovingSphere : public Hitable
{
public:
	MovingSphere() {}
	~MovingSphere() {}
	MovingSphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, Material *m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), material(m) {};
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;
	Vec3 center(float time) const;

	Vec3 center0, center1;
	float time0, time1;
	float radius;
	Material* material;
};

Vec3 MovingSphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool MovingSphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(b*b - a*c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point(rec.t);
			get_sphere_uv((rec.p - center(r.time())) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.material = material;
			return true;
		}
		temp = (-b + sqrt(b*b - a*c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point(rec.t);
			get_sphere_uv((rec.p - center(r.time())) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.material = material;
			return true;
		}
	}

	return false;
}

bool MovingSphere::bounding_box(float t0, float t1, AABB& box) const {
	AABB box0(center(t0) - Vec3(radius, radius, radius), center(t0) + Vec3(radius, radius, radius));
	AABB box1(center(t1) - Vec3(radius, radius, radius), center(t1) + Vec3(radius, radius, radius));
	box = surrounding_box(box0, box1);
	return true;
}