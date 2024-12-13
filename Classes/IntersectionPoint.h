#ifndef INTERSECTION_POINT_H
#define INTERSECTION_POINT_H

#include <array>
#include <memory>
#include <glm/glm.hpp>  // Include GLM
// #include "Object3D.h"

class Object3D;

class IntersectionPoint {
private:
    glm::vec3 position; // The coordinates of the intersection point {x, y, z}
    double distance;                // Distance from the ray origin to the intersection point
    Object3D* object; // The object that the ray intersected
    // glm::vec3 normal;   // The normal vector at the intersection point
    // bool valid;                     // Whether this is a valid intersection (useful for missed rays)

public:
    // Constructor
    IntersectionPoint(const glm::vec3 position,
                      const double distance,
                      Object3D* object);
    // IntersectionPoint(const glm::vec3 position,
    //                   const glm::vec3 normal,
    //                   const std::shared_ptr<Object3D>& object,
    //                   const double distance,
    //                   const bool valid = true);

    static const IntersectionPoint NO_INTERSECTION;

    // Getters
    glm::vec3 getPosition() const;
    double getDistance() const;
    Object3D* getObject() const;
    bool isValid() const;
    // glm::vec3 getNormal() const;

    // Setters (Optional)
    // void setPosition(glm::vec3 position);
    // void setNormal(glm::vec3 normal);
    // void setObject(const std::shared_ptr<Object3D>& object);
    // void setDistance(double distance);
    // void setValid(bool valid);
};

#endif // INTERSECTION_POINT_H
