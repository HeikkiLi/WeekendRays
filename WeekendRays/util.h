#pragma once

#include <random>

#define _USE_MATH_DEFINES true
#include <math.h>

//#define M_PI 3.14159265358979323846

float fRandom()
{
	
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0, 1.0);

	float val = dist(mt);
	return val;
}


Vec3 random_in_unit_sphere()
{
	Vec3 p;
	do {
		p = 2.0f*Vec3(fRandom(), fRandom(), fRandom()) - Vec3(1, 1, 1);
	} while (dot(p, p) >= 1.0f);
	return p;

}

Vec3 random_on_unit_sphere()
{
	Vec3 p;
	do {
		p = 2.0f*Vec3(fRandom(), fRandom(), fRandom()) - Vec3(1, 1, 1);
	} while (dot(p, p) >= 1.0f);
	return unit_vector(p);

}

inline Vec3 random_to_sphere(float radius, float distance_squared)
{
	float r1 = fRandom();
	float r2 = fRandom();
	float z = 1 + r2*(sqrt(1 - radius*radius / distance_squared) - 1);
	float phi = 2 * M_PI*r1;
	float x = cos(phi)*sqrt(1 - z*z);
	float y = sin(phi)*sqrt(1 - z*z);
	return Vec3(x, y, z);
}

Vec3 random_in_unit_disk()
{
	Vec3 p;
	do {
		p = 2.0 * Vec3(fRandom(), fRandom(), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n)*n;
}

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted)
{
	Vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1 - dt*dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0*r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

inline float clamp(float val, float low, float hi) 
{

	if (val <= low) 
	{
		return low;
	}
	else if (val >= hi) 
	{
		return hi;
	}
	else {
		return val;
	}

}

inline Vec3 random_cosine_direction()
{
	float r1 = fRandom();
	float r2 = fRandom();
	float z = sqrt(1 - r2);
	float phi = 2 * M_PI*r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	
	return Vec3(x, y, z);
}