#include "Plane.h"
#include "IntersectionPoint.h"

Plane::Plane(const glm:: vec3 parameters, const glm:: vec3 color, const double shininess, const std::string state, const double d): 
Object3D(parameters, color, shininess, state), d(d) {}

double Plane::getD() const{
    return d;
}

IntersectionPoint Plane::intersection(Ray ray){
    return IntersectionPoint::NO_INTERSECTION;
}

std::string Plane::getName() const{
    return "Plane";
}

std::string Plane::toString() const {
    std::ostringstream oss;
    oss << "Plane: { "
        << Object3D::toString() << ", "
        << "d: " << d << " }";
    return oss.str();
}
