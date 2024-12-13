#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>  // Include GLM
#include "Object3D.h"

class Plane: public Object3D{
private:
    const double d;

public:
    // Constructor
    Plane(const glm:: vec3 parameters, const glm:: vec3 color, const double shininess, const std::string state, const double r);

    double getD() const;
    std::string getName() const override;
    std::string toString() const override;
    IntersectionPoint intersection(Ray ray) override;
    
    virtual ~Plane() = default; // virtual destructor (inherits from Object3D)
};

#endif // Plane_H