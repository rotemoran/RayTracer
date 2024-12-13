#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>  // Include GLM
#include "Object3D.h"

class Sphere: public Object3D{
private:
    const double r;

public:
    // Constructor
    Sphere(const glm::vec3 parameters, const glm::vec3 color, const double shininess, const std::string state, const double r);

    double getRadius() const;

    std::string toString() const override;

    IntersectionPoint intersection(Ray ray) override;
    std::string getName() const override;
    virtual ~Sphere() = default; // virtual destructor (inherits from Object3D)
};

#endif // SPHERE_H