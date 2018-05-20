#include <iostream>
#include <float.h>
#include <thread>

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
#include "pdf.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rdparty\stb_image.h"
#include "3rdparty\std_image_write.h"

inline Vec3 de_nan(const Vec3& c) {
	Vec3 temp = c;
	if (!(temp[0] == temp[0])) temp[0] = 0;
	if (!(temp[1] == temp[1])) temp[1] = 0;
	if (!(temp[2] == temp[2])) temp[2] = 0;
	return temp;
}

void FinalBook2(Hitable **scene, Camera **cam, float aspect)
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
			float y1 = 100 * (fRandom() + 0.01);
			float z1 = z0 + w;
			boxList[b++] = new Box(Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new BVHNode(boxList, b, 0, 1);
	Material* light = new DiffuseLight(new ConstantTexture(Vec3(7, 7, 7)));
	list[l++] = new FlipNormals( new XZRect(123, 423, 147, 412, 554, light));
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
		boxList2[j] = new Sphere(Vec3(165 * fRandom(), 165 * fRandom(), 165 * fRandom()), 10, white);
	}
	list[l++] = new Translate(new RotateY(new BVHNode(boxList2, ns, 0.0, 1.0), 15), Vec3(-100, 270, 395));

	*scene = new HitableList(list, l);
	Vec3 lookfrom(278, 278, -800);
	Vec3 lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;
	*cam = new Camera(lookfrom, lookat, Vec3(0, 1, 0),
		vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);

}

void CornellBox(Hitable **scene, Camera **cam, float aspect)
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
	
	Material *glass = new Dielectric(1.5);
	list[i++] = new Sphere(Vec3(190, 90, 190), 90, glass);
	
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


void RandomScene(Hitable **scene, Camera **cam, float aspect)
{

	Vec3 lookfrom(13, 2, 3);
	Vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	*cam = new Camera(lookfrom, lookat, Vec3(0, 1, 0), 20, aspect, aperture, dist_to_focus, 0.0, 1.0);

	int n = 500;
	Hitable **list = new Hitable*[n + 1 +1];
	Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
											new ConstantTexture(Vec3(0.9, 0.9, 0.9)));

	
	list[0] = new Sphere(Vec3(0, -1000.0, 0), 1000, new Lambertian(checker));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			float choose_mat = fRandom();
			Vec3 center(a + 0.9*fRandom(), 0.2, b + 0.9*fRandom());
			if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.1) {  // diffuse
					list[i++] = new Sphere(center, 0.2,
							new Lambertian(new ConstantTexture(Vec3(fRandom()*fRandom(), fRandom()*fRandom(), fRandom()*fRandom()))) );
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new Sphere(center, 0.2,
						new Metal(Vec3(0.5*(1 + fRandom()), 0.5*(1 + fRandom()), 0.5*(1 + fRandom())), 0.5*fRandom()));
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

	Material *light = new DiffuseLight(new ConstantTexture(Vec3(15, 15, 15)));
	list[i++] = new FlipNormals(new XZRect(123, 423, 147, 412, 554, light));
	//return new HitableList(list, i);
	*scene = new BVHNode(list, i, 0.0, 1.0);
}

Vec3 color(const Ray& r, Hitable *world, Hitable *light_shape, int depth)
{
	HitRecord hrec;
	if (world->hit(r, 0.001f, FLT_MAX, hrec))
	{
		ScatterRecord srec;
		Vec3 emitted = hrec.material->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
		
		if (depth < 50 && hrec.material->scatter(r, hrec, srec))
		{
			if (srec.isSpecular)
			{
				return srec.attenuation * color(srec.specularRay, world, light_shape, depth + 1);
			}
			else {
				HitablePDF pLight(light_shape, hrec.p);
				MixturePDF p(&pLight, srec.pdfPtr);

				Ray scattered = Ray(hrec.p, p.generate(), r.time());
				float pdf_val = p.value(scattered.direction());
				delete srec.pdfPtr;
				return emitted + srec.attenuation*hrec.material->scattering_pdf(r, hrec, scattered)*color(scattered, world, light_shape, depth + 1) / pdf_val;
			}
		}
		else {
			return emitted;
		}
	}
	else {
		//return Vec3(0, 0, 0);
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
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

int main()
{

	int nx = 800; // 250;
	int ny = 600; // 250;
	int ns = 1000;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable *world;
	Camera *cam;
	float aspect = float(ny) / float(nx);
	//CornellBox(&world, &cam, aspect);
	RandomScene(&world, &cam, aspect);
	//FinalBook2(&world, &cam, aspect);
	Hitable *light_shape = new XZRect(123, 423, 147, 412, 554, 0); // new XZRect(213, 343, 227, 332, 554, 0);
	//Hitable *glass_sphere = new Sphere(Vec3(190, 90, 190), 90, 0);
	Hitable *a[1];
	a[0] = light_shape;
	//a[1] = glass_sphere;
	HitableList hlist(a, 1);
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Vec3 col = Vec3(0.0f, 0.0f, 0.0f);
			
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + fRandom()) / float(nx);
				float v = float(j + fRandom()) / float(ny);
				Ray r = cam->get_ray(u, v);

				col += de_nan(color(r, world, &hlist, 0));
			}
			

			col /= float(ns);
			// gamma correction
			//col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			col *= 0.02f;
		    col = pixelValue(col, 0.3f, 2.2f);
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