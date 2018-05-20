#pragma once

#include "hitable.h"
#include "onb.h"

class Sphere: public Hitable
{
public:
	Sphere() {}
	~Sphere() {}
	Sphere(Vec3 cen, float r, Material *m): center(cen), radius(r), material(m) {};
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;
	virtual float pdf_value(const Vec3& o, const Vec3& v) const;
	virtual Vec3 random(const Vec3& o) const;

	Vec3 center;
	float radius;
	Material* material;
};

inline float Sphere::pdf_value(const Vec3 & o, const Vec3 & v) const
{
	HitRecord rec;
	if (this->hit(Ray(o, v), 0.001, FLT_MAX, rec))
	{
		float cos_theta_max = sqrt(1 - radius*radius / (center - o).squared_length());
		float solid_angle = 2 * M_PI*(1 - cos_theta_max);
		return 1 / solid_angle;
	}
	else
		return 0.0f;
}

inline Vec3 Sphere::random(const Vec3 & o) const
{
	Vec3 direction = center - o;
	float distance_squared = direction.squared_length();
	ONB uvw;
	uvw.build_from_w(direction);
	return uvw.local(random_to_sphere(radius, distance_squared));
}

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Vec3 oc = r.origin() - center;
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
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center) / radius;
			rec.material = material;
			return true;
		}
		temp = (-b + sqrt(b*b - a*c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point(rec.t);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center) / radius;
			rec.material = material;
			return true;
		}
	}

	return false;
}

bool Sphere::bounding_box(float t0, float t1, AABB& box) const
{
	box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
	return true;
}