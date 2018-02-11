#pragma once


struct HitRecord;

#include "ray.h"
#include "hitable.h"
#include "util.h"
#include "texture.h"
#include "onb.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& albedo, Ray& scattered, float& pdf) const { return false; }
	virtual float scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const { return false; }
	virtual Vec3 emitted(float u, float v, const Vec3& p) const { return Vec3(0,0,0); }
};

class DiffuseLight : public Material
{
public:
	DiffuseLight(Texture *a) : emit(a) {}
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		return false;
	}
	virtual Vec3 emitted(float u, float v, const Vec3& p) const {
		return emit->value(u, v, p);
	}

	Texture *emit;
};

class Isotropic : public Material {
public:
	Isotropic(Texture *a) : albedo(a) {}
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		scattered = Ray(rec.p, random_in_unit_sphere());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
	Texture *albedo;
};

class Lambertian : public Material
{
public:
	Lambertian(Texture *a) : albedo(a) {}
	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& alb, Ray& scattered, float& pdf) const {
		ONB uvw;
		uvw.build_from_w(rec.normal);
		Vec3 direction = uvw.local(random_cosine_direction());
		scattered = Ray(rec.p, unit_vector(direction), r_in.time());
		alb = albedo->value(rec.u, rec.v, rec.p);
		pdf = dot(uvw.w(), scattered.direction()) / M_PI;

		return true;
	}
	float scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
		float cosine = dot(rec.normal, unit_vector(scattered.direction()));
		if (cosine < 0) cosine = 0;
		return cosine / M_PI;
	}
	Texture *albedo;
};

class Metal : public Material
{
public:
	Metal(const Vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	Vec3 albedo;
	float fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflect_prob = 0.0f;
		float cosine = 0.0f;
		if (dot(r_in.direction(), rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			scattered = Ray(rec.p, reflected);
			reflect_prob = 1.0f;
		}

		if(random() < reflect_prob)
		{
			scattered = Ray(rec.p, reflected);
		}
		else {
			scattered = Ray(rec.p, refracted);
			
		}
		return true;
	}

	float ref_idx;
};