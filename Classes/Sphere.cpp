#include "Sphere.h"
#include "IntersectionPoint.h"

Sphere::Sphere(const glm:: vec3 parameters, const glm:: vec3 color, const double shininess, const std::string state, const double r): 
Object3D(parameters, color, shininess, state), r(r)  {}

double Sphere::getRadius() const{
    return r;
}
std::string Sphere::getName() const{
    return "Sphere";
}

IntersectionPoint Sphere::intersection(Ray ray){
    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    float a = glm::dot(ray.getDirection(), ray.getDirection());
    float b = 2 * glm::dot(ray.getPosition(), ray.getDirection());
    float c = glm::dot(ray.getPosition(), ray.getPosition()) - std::pow(r,2);

    float discriminat = b * b - 4 * a * c;
    glm:: vec3 distance (ray.getPosition() - glm::vec3(1,1,1));
    IntersectionPoint hitPoint(glm::vec3(1,1,1), std::sqrt(glm::dot(distance,distance)), this);

    if (discriminat >= 0){
        return hitPoint;
    }
    else{
        return IntersectionPoint::NO_INTERSECTION;
    }

    // TODO: CONTINUE!!! FIND INTERSECTION POINT
}

std::string Sphere::toString() const {
    std::ostringstream oss;
    oss << "Sphere: { "
        << Object3D::toString() << ", "
        << "Radius: " << r << " }";
    return oss.str();
}
