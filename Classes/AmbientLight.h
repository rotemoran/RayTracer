#ifndef AmbientLight_H
#define AmbientLight_H

#include <iostream>
#include <string>
#include <array>  
#include <glm/glm.hpp> // Include GLM

class AmbientLight{
    private:
        glm::vec3 intensity;

    public:
        AmbientLight(const glm::vec3 intensity);
        glm::vec3 getIntensity() const;
        
        std::string toString() const;
};

#endif // AmbientLight_H