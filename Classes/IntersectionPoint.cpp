#include "IntersectionPoint.h"
const IntersectionPoint IntersectionPoint::NO_INTERSECTION = IntersectionPoint(glm::vec3(0,0,0), -1, nullptr);

// Constructor
IntersectionPoint::IntersectionPoint(const glm::vec3 position,
                                    const double distance,
                                    Object3D* object)
    : position(position), distance(distance), object(object){}
// IntersectionPoint::IntersectionPoint(const glm::vec3 position,
//                                     const glm::vec3 normal,
//                                     const std::shared_ptr<Object3D>& object,
//                                     const double distance,
//                                     const bool valid)
//     : position(position), normal(normal), object(object), distance(distance), valid(valid) {}

// Getters
glm::vec3 IntersectionPoint::getPosition() const {
    return position;
}

// glm::vec3 IntersectionPoint::getNormal() const {
//     return normal;
// }

double IntersectionPoint::getDistance() const {
    return distance;
}

Object3D* IntersectionPoint::getObject() const {
    return object;
}


bool IntersectionPoint::isValid() const {
    return distance >= 0;
}

// // Setters (Optional)
// void IntersectionPoint::setPosition(glm::vec3 position) {
//     this->position = position;
// }

// void IntersectionPoint::setNormal(glm::vec3 normal) {
//     this->normal = normal;
// }

// void IntersectionPoint::setObject(const std::shared_ptr<Object3D>& object) {
//     this->object = object;
// }

// void IntersectionPoint::setDistance(double distance) {
//     this->distance = distance;
// }

// void IntersectionPoint::setValid(bool valid) {
//     this->valid = valid;
// }
