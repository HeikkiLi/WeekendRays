#pragma once

#include "onb.h"
#include "util.h"

class PDF {
public:
	virtual float value(const Vec3& direction) const = 0;
	virtual Vec3 generate() const = 0;
};

class CosinePDF : public PDF
{
public:
	CosinePDF(const Vec3& w) { uvw.build_from_w(w); }
	virtual float value(const Vec3& direction) const {
		float cosine = dot(unit_vector(direction), uvw.w());
		if (cosine > 0)
			return cosine / M_PI;
		else
			return 0;
	}

	virtual Vec3 generate() const {
		return uvw.local(random_cosine_direction());
	}
	

	ONB uvw;
};

class HitablePDF : public PDF
{
public:
	HitablePDF(Hitable *p, const Vec3& origin): ptr(p), o(origin) {}
	virtual float value(const Vec3& direction) const {
		return ptr->pdf_value(o, direction);
	}

	virtual Vec3 generate() const {
		return ptr->random(o);
	}


	Vec3 o;
	Hitable *ptr;
};

class MixturePDF : public PDF
{
public:
	MixturePDF(PDF *p0, PDF *p1) { p[0] = p0; p[1] = p1; }
	virtual float value(const Vec3& direction) const {
		return 0.5 * p[0]->value(direction) + 0.5*p[1]->value(direction);
	}

	virtual Vec3 generate() const {
		if (fRandom() < 0.5)
			return p[0]->generate();
		else
			return p[1]->generate();
	}


	PDF *p[2];
};