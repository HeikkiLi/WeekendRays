#pragma once

#include "texture.h"

class ImageTexture : public Texture
{
public:
	ImageTexture() {}
	ImageTexture(unsigned char* pixels, int A, int B) : data(pixels), nx(A), ny(B) {}
	virtual Vec3 value(float u, float v, const Vec3& p) const;

	unsigned char* data;
	int nx, ny;
};

Vec3 ImageTexture::value(float u, float v, const Vec3 & p) const
{
	int i = (u)*nx;
	int j = (1 - v)*ny - 0.001;
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > nx - 1) i = nx - 1;
	if (j > ny - 1) j = ny - 1;
	float r = int(data[3*i + 3*nx*j]) / 255.0;
	float g = int(data[3*i + 3*nx*j+1]) / 255.0;
	float b = int(data[3*i + 3*nx*j+2]) / 255.0;
	return Vec3(r,g,b);
}