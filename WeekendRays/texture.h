#pragma once

#include "vec3.h"
#include "perlin.h"

class Texture
{
public:
	virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
	ConstantTexture() {}
	ConstantTexture(Vec3 c) : color(c) {}
	virtual Vec3 value(float u, float v, const Vec3& p) const { return color; }

	Vec3 color;

};

class CheckerTexture : public Texture
{
public:
	CheckerTexture() {};
	CheckerTexture(Texture *t0, Texture *t1) : even(t0), odd(t1) {}
	virtual Vec3 value(float u, float v, const Vec3& p) const {
		float sines = sin(10 * p.x())*sin(10 * p.y())*sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}


	Texture *odd;
	Texture *even;
};

class NoiseTexture : public Texture
{
public:
	NoiseTexture() { scale = 1.0f; };
	NoiseTexture(float sc): scale(sc) {};
	virtual Vec3 value(float u, float v, const Vec3& p) const {
		//  return vec3(1,1,1)*0.5*(1 + noise.turb(scale * p));
		//  return vec3(1,1,1)*noise.turb(scale * p);
		return Vec3(1, 1, 1)*0.5*(1 + sin(scale*p.x() + 5 * noise.turb(scale*p)));
	}

	Perlin noise;
	float scale;
};