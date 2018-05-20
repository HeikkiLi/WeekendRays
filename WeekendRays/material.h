#pragma once


struct HitRecord;

#include "ray.h"
#include "hitable.h"
#include "util.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

struct ScatterRecord
{
	Ray specularRay;
	bool isSpecular;
	Vec3 attenuation;
	PDF* pdfPtr;
};

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const { return false; }
	virtual float scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const { return false; }
	virtual Vec3 emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Vec3& p) const { return Vec3(0, 0, 0); }		
};

class DiffuseLight : public Material
{
public:
	DiffuseLight(Texture *a) : emit(a) {}

	virtual Vec3 emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Vec3& p) const {
		if (dot(rec.normal, r_in.direction()) < 0.0)
			return emit->value(u, v, p);
		else
			return Vec3(0, 0, 0);
	}
	

	Texture *emit;
};


class Isotropic : public Material {
public:
	Isotropic(Texture *a) : albedo(a) {}
	virtual bool scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const {
		Vec3 reflected = reflect(unit_vector(r_in.direction()), hrec.normal);
		srec.specularRay = Ray(hrec.p, reflected + random_in_unit_sphere());
		srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);;
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		return true;
	}
	Texture *albedo;
};


class Lambertian : public Material
{
public:
	Lambertian(Texture *a) : albedo(a) {}
	bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const {
		srec.isSpecular = false;
		srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
		srec.pdfPtr = new CosinePDF(rec.normal);
		return true;
	}
	float scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
		float cosine = dot(rec.normal, unit_vector(scattered.direction()));
		
		if (cosine < 0)
			return 0;

		return cosine / M_PI;
	}
	Texture *albedo;
};


class Metal : public Material
{
public:
	Metal(const Vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const {
		Vec3 reflected = reflect(unit_vector(r_in.direction()), hrec.normal);
		srec.specularRay = Ray(hrec.p, reflected + fuzz*random_in_unit_sphere());
		srec.attenuation = albedo;
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		return true;
	}

	Vec3 albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const {
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		srec.attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction(), hrec.normal);
		Vec3 refracted;
		float ni_over_nt;
		float reflect_prob;
		float cosine;
		if (dot(r_in.direction(), hrec.normal) > 0) {
			outward_normal = -hrec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), hrec.normal) / r_in.direction().length();
		}
		else {
			outward_normal = hrec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), hrec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			reflect_prob = 1.0;
		}
		if (fRandom() < reflect_prob) {
			srec.specularRay = Ray(hrec.p, reflected);
		}
		else {
			srec.specularRay = Ray(hrec.p, refracted);
		}
		return true;
	}

	float ref_idx;
};