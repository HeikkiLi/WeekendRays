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

#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty\stb_image.h"


Hitable *CornellBox()
{
	Hitable **list = new Hitable*[6];
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
	return new HitableList(list, i);
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
				if (choose_mat < 0.8) {  // diffuse
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
		Vec3 emitted = rec.material->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered))
		{
			return emitted + attenuation*color(scattered, world, depth + 1);
		}
		else {
			return emitted;
		}
	}
	else {
		return Vec3(0, 0, 0);
		/*
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		return (1.0f - t)*Vec3(1.0f, 1.0f, 1.0f) + t*Vec3(0.5f, 0.7f, 1.0f);
		*/
	}
}

int main()
{
	int nx = 400; // 200;
	int ny = 400; // 100;
	int ns = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	//Hitable *world = RandomScene();
	//Hitable *world = TwoSpheres();
	//Hitable *world = TwoPerlinSpheres();
	//Hitable *world = Earth();
	//Hitable *world = simpleLight();
	Hitable *world = CornellBox();
	//Vec3 lookfrom(13, 2, 3); // Random scene
	//Vec3 lookfrom(0, 2, 10); // Earth scene
	//Vec3 lookfrom(8, 3, 8);  //  Simple light scene
	
	Vec3 lookfrom(278, 278, -800);  // CornellBox
	Vec3 lookat(278, 278, 0);		// CornellBox

	//Vec3 lookat(0, 2, 0);		
	//Vec3 lookat(0, 0, 0);
	
	float dist_to_focus = 10.0; // (lookfrom - lookat).length();
	float aperture = 0.0;
	float vfov = 40.0;
	Camera cam(lookfrom, lookat, Vec3(0, 1, 0), vfov, float(nx)/float(ny), aperture, dist_to_focus,0.0, 1.0);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Vec3 col = Vec3(0.0f, 0.0f, 0.0f);
			
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + random()) / float(nx);
				float v = float(j + random()) / float(ny);
				Ray r = cam.get_ray(u, v);

				Vec3 p = r.point(2.0);
				col += color(r, world, 0);
			}
			

			col /= float(ns);
			// gamma correction
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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