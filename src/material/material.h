#ifndef MATERIAL_H
#define MATERIAL_H

#include "../utility/rtweekend.h"
#include "../hittable_objects/hittable.h"
#include "../utility/color.h"

class material
{
public:
    virtual ~material() = default;
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
    virtual bool emmited(color &attenuation) const { return false; }
};

class lambertian : public material
{
public:
    lambertian(const color &a) : albedo(a) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {

        auto temp_vec = random_unit_vector();

        // while (temp_vec.near_zero())
        //     temp_vec = random_unit_vector();

        if (temp_vec.near_zero())
            temp_vec = rec.normal;

        auto scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class diffuse_light : public material
{
public:
    diffuse_light(const color &a): albedo(a) {}
    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        return false;
    }

    bool emmited(color &attenuation) const override
    {
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material
{
public:
    metal(const color &a, double _fuz) : albedo(a), fuz(_fuz < 1 ? _fuz : 1 ) {}
 
    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        auto reflected_dir = reflect(unit_vector(r_in.direction()), rec.normal);

        // if (reflected_dir.near_zero())
        //     reflected_dir = rec.normal;

        scattered = ray(rec.p, reflected_dir + fuz * random_unit_vector());
        attenuation = albedo;

        return (dot(scattered.direction(), rec.normal) > 0); // 0 for no pertubation case
    }
private:
    color albedo;
    double fuz;
};


class dielectric : public material
{

    public:
        dielectric(double ri) : ref_idx(ri) {}

        bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)const override{

            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0 / ref_idx) : ref_idx;
            vec3 unit_direction = unit_vector(r_in.direction());

            // Total internal reflection condition
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        double ref_idx;

        static double reflectance(double cosine, double ref_idx)
        {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};
#endif