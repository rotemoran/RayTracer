#ifndef CAMERA_H
#define CAMERA_H

#include <array>
#include <vector>
#include <cfloat>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "Object3D.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "IntersectionPoint.h"
#include "Spotlight.h"
#include "Sphere.h"
#include "Plane.h"
#include "Ray.h"


extern const int SCREEN_DIM; // resolution
extern const double RATIO; // pixel width
extern glm::vec3 P_C; // image center 
extern glm::vec3 V_TOWARDS; 
extern glm::vec3 V_UP;
extern glm::vec3 V_RIGHT;


class Camera {
private:
    glm::vec3 cameraPosition;     // Represents {x, y, z}
    bool multiSampling;                 // BONUS feature        
    std::vector<std::shared_ptr<Object3D>> objects; // Objects in the scene
    std::vector<std::shared_ptr<AmbientLight>> ambientLight; // Ambient Light in the scene
    std::vector<std::shared_ptr<DirectionalLight>> directionalLights; // Directional lights
    std::vector<std::shared_ptr<Spotlight>> spotlights; // Spotlights 
    cv::Mat screen; 

    void parseConFile(const std::string& filename); // Parse configuration file

public:
    // Constructor
    Camera(const std::string& filename);

    // Accessors
    glm::vec3 getPosition() const;
    bool isMultiSamplingEnabled() const;

    // Getters for objects and lights
    const std::vector<std::shared_ptr<Object3D>>& getObjects() const;
    const std::vector<std::shared_ptr<AmbientLight>>& getAmbientLight() const;
    const std::vector<std::shared_ptr<DirectionalLight>>& getDirectionalLights() const;
    const std::vector<std::shared_ptr<Spotlight>>& getSpotlights() const;
    void createScene();
    void toString();
    IntersectionPoint findIntersectionObj(Ray ray) const;
    glm::vec3 getColor(Ray shootingRay) const;
    void insertSorted(std::shared_ptr<Object3D> newObject);
};

#endif // CAMERA_H
