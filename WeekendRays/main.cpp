#include <iostream>
#include <float.h>

#include "sphere.h"
#include "movingsphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "util.h"
#include "material.h"
#include "bvh.h"
#include "ImageTexture.h"
#include "aarect.h"
#include "box.h"
#include "ConstantMedium.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rdparty\stb_image.h"
#include "3rdparty\std_image_write.h"

Hitable* FinalBook2()
{
	int nb = 20;
	Hitable** list = new Hitable*[30];
	Hitable** boxList = new Hitable*[10000];
	Hitable** boxList2 = new Hitable*[10000];
	Material* white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
	Material* ground = new Lambertian(new ConstantTexture(Vec3(0.48, 0.83, 0.53)));

	int b = 0;
	for(int i=0; i<nb; i++)
	{ 
		for (int j = 0; j < nb; j++)
		{
			float w = 100;
			float x0 = -1000 + i*w;
			float z0 = -1000 + j*w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (random() + 0.01);
			float z1 = z0 + w;
			boxList[b++] = new Box(Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new BVHNode(boxList, b, 0, 1);
	Material* light = new DiffuseLight(new ConstantTexture(Vec3(7, 7, 7)));
	list[l++] = new XZRect(123, 423, 147, 412, 554, light);
	Vec3 center(400, 400, 200);
	list[l++] = new MovingSphere(center, center + Vec3(30, 0, 0), 0, 1, 50,
		new Lambertian(new ConstantTexture(Vec3(0.7, 0.3, 0.1))));
	list[l++] = new Sphere(Vec3(260, 150, 45), 50, new Dielectric(1.5));
	list[l++] = new Sphere(Vec3(0, 150, 145), 50, new Metal(Vec3(0.8,0.8,0.9), 10.0));
	Hitable* boundary = new Sphere(Vec3(360, 150, 145), 70, new Dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new ConstantMedium(boundary, 0.2, new ConstantTexture(Vec3(0.2, 0.4, 0.9)));
	boundary = new Sphere(Vec3(0, 0, 0), 5000, new Dielectric(1.5));
	list[l++] = new ConstantMedium(boundary, 0.0001, new ConstantTexture(Vec3(1.0, 1.0, 1.0)));
	int nx, ny, nn;
	unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	Material* emat = new Lambertian(new ImageTexture(tex_data, nx, ny));
	list[l++] = new Sphere(Vec3(400, 200, 400), 100, emat);
	Texture* pertext = new NoiseTexture(0.1);
	list[l++] = new Sphere(Vec3(220, 280, 300), 80, new Lambertian(pertext));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxList2[j] = new Sphere(Vec3(165 * random(), 165 * random(), 165 * random()), 10, white);
	}
	list[l++] = new Translate(new RotateY(new BVHNode(boxList2, ns, 0.0, 1.0), 15), Vec3(-100, 270, 395));
	return new HitableList(list, l);
}

Hitable* CornellBalls()
{
	Hitable **list = new Hitable*[9];
	int i = 0;
	Material *red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(Vec3(5, 5, 5)));

	// Walls
	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new FlipNormals(new XZRect(113, 443, 127, 432, 554, light) );
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));

	// Spheres (Balls)
	Hitable *boundary = new Sphere(Vec3(160, 100, 145), 100, new Dielectric(1.5));
	list[i++] = boundary;
	list[i++] = new ConstantMedium(boundary, 0.1, new ConstantTexture(Vec3(1.0, 1.0, 1.0)));
	list[i++] = new Translate(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295));

	return new HitableList(list, i);
}

Hitable* CornellSmoke()
{
	Hitable **list = new Hitable*[8];
	int i = 0;
	Material *red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(Vec3(17,17, 17)));

	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new XZRect(113, 443, 127, 432, 554, light);
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));

	Hitable *b1 = new Translate(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130, 0, 65));
	Hitable *b2 = new Translate(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295));
	list[i++] = new ConstantMedium(b1, 0.01, new ConstantTexture(Vec3(1.0, 1.0, 1.0)));
	list[i++] = new ConstantMedium(b2, 0.01, new ConstantTexture(Vec3(0.0, 0.0, 0.0)));
	
	return new HitableList(list, i);
}

Hitable* XZRectTest()
{
	Hitable **list = new Hitable*[2];
	int i = 0;
	Material *light = new DiffuseLight(new ConstantTexture(Vec3(1, 1, 1)));
	list[i++] = new XZRect(113, 443, 127, 432, 554, light);
	list[i++] = new XYRect(113, 443, 127, 432, 554, light);
	return new HitableList(list, i);
}

Hitable *CornellBox()
{
	Hitable **list = new Hitable*[8];
	int i = 0;
	Material *red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(Vec3(15, 15, 15)));
	
	list[i++] = new FlipNormals( new YZRect(0, 555, 0, 555, 555, green) );
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new XZRect(213, 343, 227, 332, 554, light);
	list[i++] = new FlipNormals( new XZRect(0, 555, 0, 555, 555, white) );
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormals( new XYRect(0, 555, 0, 555, 555, white) );
	list[i++] = new Translate( new RotateY( new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130, 0, 65));
	list[i++] = new Translate( new RotateY( new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265,0,295));
	return new HitableList(list, i);
}

void CornellBox_MC(Hitable **scene, Camera **cam, float aspect)
{
	int i = 0;
	Hitable **list = new Hitable*[8];
	Material *red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(Vec3(15, 15, 15)));
	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new FlipNormals(new XZRect(213, 343, 227, 332, 554, light));
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
	
	list[i++] = new Translate(new RotateY(
		new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130, 0, 65));
	list[i++] = new Translate(new RotateY(
		new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295));
	*scene = new HitableList(list, i);
	Vec3 lookfrom(278, 278, -800);
	Vec3 lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;
	*cam = new Camera(lookfrom, lookat, Vec3(0, 1, 0),
					vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);
}

Hitable* simpleLight()
{
	Texture *pertext = new NoiseTexture(4);
	Hitable **list = new Hitable*[4];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(pertext));
	list[1] = new Sphere(Vec3(0, 2, 0), 2, new Lambertian(pertext));
	list[2] = new Sphere(Vec3(0, 7, 0), 2, new DiffuseLight(new ConstantTexture(Vec3(4, 4, 4))));
	list[3] = new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(Vec3(4, 4, 4))));
	return new HitableList(list, 4);
}

Hitable* Earth()
{
	int nx = 0;
	int ny = 0;
	int nn = 0;
	unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	Material *mat = new Lambertian(new ImageTexture(tex_data, nx, ny));
	return new Sphere(Vec3(0, 0, 0), 2, mat);
}

Hitable* TwoPerlinSpheres()
{
	Texture *pertext = new NoiseTexture(4);
	Hitable **list = new Hitable*[2];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian( pertext ));
	list[1] = new Sphere(Vec3(0, 2, 0), 2, new Lambertian( pertext ));

	return new HitableList(list, 2);
}

Hitable* TwoSpheres()
{
	Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
		new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
	Hitable **list = new Hitable*[2];
	list[0] = new Sphere(Vec3(0, -10, 0), 10, new Lambertian(checker));
	list[1] = new Sphere(Vec3(0, 10, 0), 10, new Lambertian(checker));

	return new HitableList(list, 2);
}

Hitable* RandomScene()
{
	int n = 500;
	Hitable **list = new Hitable*[n + 1];
	Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
											new ConstantTexture(Vec3(0.9, 0.9, 0.9)));

	
	list[0] = new Sphere(Vec3(0, -1000.0, 0), 1000, new Lambertian(checker));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random();
			Vec3 center(a + 0.9*random(), 0.2, b + 0.9*random());
			if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.1) {  // diffuse
					list[i++] = new MovingSphere(center, center+Vec3(0,0.5, random()), 0.0, 1.0, 0.2, 
							new Lambertian(new ConstantTexture(Vec3(random()*random(), random()*random(), random()*random()))) );
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new Sphere(center, 0.2,
						new Metal(Vec3(0.5*(1 + random()), 0.5*(1 + random()), 0.5*(1 + random())), 0.5*random()));
				}
				else {  // glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(Vec3(0.4, 0.2, 0.1))) );
	list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

	//return new HitableList(list, i);
	return new BVHNode(list, i, 0.0, 1.0);
}

Vec3 color(const Ray& r, Hitable *world, int depth)
{
	HitRecord rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		Ray scattered;
		Vec3 attenuation;
		Vec3 emitted = rec.material->emitted(r, rec, rec.u, rec.v, rec.p);
		float pdf;
		Vec3 albedo;
		if (depth < 50 && rec.material->scatter(r, rec, albedo, scattered, pdf))
		{
			Vec3 onLight = Vec3(213 + random()*(343 - 213), 554, 227 + random()*(332 - 227));
			Vec3 toLight = onLight - rec.p;
			float distanceSquared = toLight.squared_length();
			toLight.make_unit_vector();
			if (dot(toLight, rec.normal) < 0)
				return emitted;
			float lightArea = (343 - 213)*(332 - 227);
			float lightCosine = fabs(toLight.y());
			if (lightCosine < 0.000001)
				return emitted;
			pdf = distanceSquared / (lightCosine*lightArea);
			scattered = Ray(rec.p, toLight, r.time());

			return emitted + albedo*rec.material->scattering_pdf(r,rec,scattered)*color(scattered, world, depth + 1) / pdf;
		}
		else {
			return emitted;
		}
	}
	else {
		return Vec3(0, 0, 0);
	}
}

Vec3 pixelValue(const Vec3 radiance, const float k, const float gamma)
{
	// Adjust for constant sensitivity
	Vec3 L = radiance;
	L *= 1.0f / k;

	// Maximum radiance at any frequency
	float m = fmax(fmaxf(L.r(), L.g()), fmaxf(L.b(), 1.0f));

	// Normalize the input
	L *= 1.0f / m;

	
	//Restore magnitude, but fade towards white when the maximum value approaches 1.0
	m = clamp((m - 1.0f) * 0.2f, 0.0f, 1.0f);
	L = L * (1.0 - m) + Vec3(m, m, m);

	//Gamma encode for a sRGB display
	Vec3 c = Vec3(L.r(), L.g(), L.b());

	c[0] = pow(c.r(), 1.0f / gamma);
	c[1]= pow(c.g(), 1.0f / gamma);
	c[2]= pow(c.b(), 1.0f / gamma);
	return c;
}

// Probability Density Function
inline float pdf(const Vec3& p)
{
	return 1 / (4 * M_PI);
}

int main()
{

	int nx = 256; // 200;
	int ny = 256; // 100;
	int ns = 10;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable *world;
	Camera *cam;
	float aspect = float(ny) / float(nx);
	CornellBox_MC(&world, &cam, aspect);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Vec3 col = Vec3(0.0f, 0.0f, 0.0f);
			
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + random()) / float(nx);
				float v = float(j + random()) / float(ny);
				Ray r = cam->get_ray(u, v);

				Vec3 p = r.point(2.0);
				col += color(r, world, 0);
			}
			

			col /= float(ns);
			// gamma correction
			//col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			col = pixelValue(col, 0.9f, 2.2f);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";

		}
	}
	

	if (world != NULL)
	{
		delete world;
		world = NULL;
	}
}