#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
// #include "IntersectionPoint.h"
#include "Ray.h"

class IntersectionPoint;

class Object3D{
    private:
        const glm::vec3 parameters; // Represents {x, y, z}
        const glm::vec3 color;  // Represents color {r, g, b}
        const double shininess; 
        const std::string state; // "n" for normal, "r" for reflective, "t" for transparent

    public:
        // Constructor
        Object3D(const glm:: vec3 parameters,const glm:: vec3 color, const double shininess, const std::string state);

        glm:: vec3 getParameters() const;
        glm:: vec3 getColor() const;
        double getShininess() const;
        std::string getState() const;
        double get1Pram() const;
        double get2Pram() const;
        double get3Parm() const;

        virtual std::string toString() const;
        virtual std::string getName() const;
        virtual IntersectionPoint intersection(Ray ray) = 0; 

        virtual ~Object3D() = default; // virtual destructor
};

#endif // OBJECT3D_H