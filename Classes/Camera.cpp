# include "Camera.h" 

const int SCREEN_DIM = 800; // resolution
const double SCREEN_WIDTH = 2;
const double RATIO = SCREEN_WIDTH / SCREEN_DIM; // pixel width
glm::vec3 P_C(0, 0, 0); // image center
glm::vec3 V_TOWARDS(0, 0, -1);
glm::vec3 V_UP(0, 1, 0);
glm::vec3 V_RIGHT = glm::cross(V_TOWARDS, V_UP);

Camera::Camera(const std::string& filename){
    screen = cv::Mat::zeros(SCREEN_DIM, SCREEN_DIM, CV_8UC3);
    parseConFile(filename);
    // defining camera view variables
    // int screenW =  rightUpCorner[0] - leftBottomCorner[0]; // world-space width
    // int screenH =  rightUpCorner[1] - leftBottomCorner[1]; // world-space height
    // screenDistance = position[3]; // camera's distance from scene
    // fieldOfView = 2 * atan2(screenW / (2 * screenDistance)); // verticl FOV = horizontal FOV
    // pixelSize = screenW / SCREEN_DIM; // Pixel width = Pixel height
    std::cout << "Got before create scene: " << std::endl;
    createScene();
}

void Camera::parseConFile(const std::string& filename){  
    std::vector<std::vector<std::string>> elements;
    std::vector<std::vector<std::string>> colors;
    std::vector<std::vector<std::string>> lights;
    std::vector<std::vector<std::string>> intensities;
    std::vector<std::vector<std::string>> lightsPositions;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "e") {
            // Camera (Eye Position)
            std::string multiSamplingFlag;
            double x, y, z;
            iss >> x >> y >> z >> multiSamplingFlag;
            cameraPosition = glm::vec3(x, y, z);
            multiSampling = (multiSamplingFlag != "0.0"); // 0.0 = no, 1.0 = yes
        } 
        else if (type == "a") {
            // Global Ambient Light
            double r, g, b, unused;
            iss >> r >> g >> b >> unused;
            ambientLight.push_back(std::make_shared<AmbientLight>(glm::vec3(r, g, b)));
        }
        else if (type == "o" || type == "r" || type == "t") {
            // Object (Sphere or Plane)
            std::vector<std::string> element(4);
            iss >> element[0] >> element[1] >> element[2] >> element[3];
            elements.push_back({type, element[0], element[1], element[2], element[3]});
        }
        else if (type == "c") {
            // Color and Shininess
            std::vector<std::string> element(4);
            iss >> element[0] >> element[1] >> element[2] >> element[3];
            colors.push_back({element[0], element[1], element[2], element[3]});
        }
        else if (type == "d") {
            // Directional Light
            std::vector<std::string> element(4);
            iss >> element[0] >> element[1] >> element[2] >> element[3];
            lights.push_back({element[0], element[1], element[2], element[3]});
        } 
        else if (type == "p") {
            // Spotlight Position
            std::vector<std::string> element(4);
            iss >> element[0] >> element[1] >> element[2] >> element[3];
            lightsPositions.push_back({element[0], element[1], element[2], element[3]});
        } 
        else if (type == "i") {
            // Light Intensity
            std::vector<std::string> element(3);
            iss >> element[0] >> element[1] >> element[2];
            intensities.push_back({element[0], element[1], element[2]});
        }
    }
    
    // elments creation
    for (unsigned long i = 0; i < elements.size(); i++) {
        std::vector<std::string> element = elements[i];
        std::vector<std::string> color = colors[i];
        // Convert parameters and color to glm::vec3
        glm::vec3 params(std::stod(element[1]), std::stod(element[2]), std::stod(element[3]));
        glm::vec3 objColor(std::stod(color[0]), std::stod(color[1]), std::stod(color[2]));

        std::shared_ptr<Object3D> newObject;
        if (std::stod(element[4]) > 0) {
            // Create Sphere object
            newObject = std::make_shared<Sphere>(
                params,             // glm::vec3 parameters
                objColor,           // glm::vec3 color
                std::stod(color[3]), // Shininess
                element[0],         // State
                std::stod(element[4]) // Radius
            );
        } 
        else {// Create Plane object
            newObject = std::make_shared<Plane>(
                params,             // glm::vec3 parameters
                objColor,           // glm::vec3 color
                std::stod(color[3]), // Shininess
                element[0],         // State
                std::stod(element[4]) // Radius (interpreted as thickness or another parameter)
            );
        }
        insertSorted(newObject);
    }
    int spotlightNextIndx = 0;
    for (unsigned long i = 0; i < lights.size(); i++) {
        std::vector<std::string> light = lights[i];
        std::vector<std::string> intensity = intensities[i];

        // Convert light direction and intensity to glm::vec3
        glm::vec3 lightDirec(std::stod(light[0]), std::stod(light[1]), std::stod(light[2]));
        glm::vec3 lightInten(std::stod(intensity[0]), std::stod(intensity[1]), std::stod(intensity[2]));

        // Spotlight = 1.0, Directional = 0.0
        if (std::stod(light[3]) == 0.0) {
            // Create Directional Light
            directionalLights.push_back(std::make_shared<DirectionalLight>(
                lightInten, // Intensity
                lightDirec  // Direction
            ));
        } 
        else {// Create Spotlight
            std::vector<std::string> pos = lightsPositions[spotlightNextIndx];
            glm::vec3 lightPos(std::stod(pos[0]), std::stod(pos[1]), std::stod(pos[2]));
            spotlightNextIndx++;

            spotlights.push_back(std::make_shared<Spotlight>(
                lightInten,          // Intensity
                lightDirec,          // Direction
                lightPos,            // Position
                std::stod(pos[3])    // Cutoff angle cosine value
            ));
        }
    }
    file.close();
}


// find the intersection point to the closest obj
IntersectionPoint Camera::findIntersectionObj(Ray ray) const {
    float minDist = FLT_MAX;
    IntersectionPoint minPoint = IntersectionPoint::NO_INTERSECTION;

    // Iterate through all objects in the scene
    for (const std::shared_ptr<Object3D>& obj : objects) { 
        IntersectionPoint intersectionPoint = obj->intersection(ray);

        // If the intersection exists and is closer than the current min distance
        if (intersectionPoint.isValid() && minDist > intersectionPoint.getDistance()) {
            minDist = intersectionPoint.getDistance();
            minPoint = intersectionPoint;
        }
    }

    return minPoint;
}

glm:: vec3 Camera::getColor(Ray shootingRay) const{
    bool flag = false;
    // int iterations = 0;
    Ray ray = shootingRay;

    while(!flag){ // CONTINUE!!!
        IntersectionPoint intersectedRes = findIntersectionObj(ray);
        if (intersectedRes.isValid()){
        //     std::cout << "intersectedRes: " << std::endl;
        //     std::cout << "  Position: (" 
        //   << intersectedRes.getObject()->getParameters().x << ", "
        //   << intersectedRes.getObject()->getParameters().y << ", "
        //   << intersectedRes.getObject()->getParameters().z << ")" << std::endl;

            return glm::vec3(128,128,128);
        }
        return glm::vec3(0,0,0);
    }
    return glm::vec3(0,0,0);
}

std::string Object3D::getName() const {
    return "Base Object";  // Example default implementation
}

void Camera::createScene(){
    // Double for loop
    // screen = cv::Mat::zeros(800, 800, CV_8UC3); in the constroctor
    for (int row = 0; row < screen.rows; row++) {
        for (int col = 0; col < screen.cols; col++) {
            glm::vec3 pixelCenter = P_C + 
                                    float((row - floor(SCREEN_DIM/2))* RATIO) * V_RIGHT 
                                    - float((col - floor(SCREEN_DIM/2))* RATIO) * V_UP ;
            if((pixelCenter.x < -0.99 && pixelCenter.y < -0.99)  || (pixelCenter.x > 0.99 && pixelCenter.y > 0.99))
                std::cout << "pixelCenter = (" 
                        << pixelCenter.x << ", " 
                        << pixelCenter.y << ", " 
                        << pixelCenter.z << ")" 
                        << std::endl;

            Ray pixelRay(pixelCenter-cameraPosition, cameraPosition);
            glm::vec3 color = getColor(pixelRay);
            screen.at<cv::Vec3b>(row, col) = cv::Vec3b(
            static_cast<uchar>(color.b), 
            static_cast<uchar>(color.g), 
            static_cast<uchar>(color.r)
            );
        }
    }
    cv::imshow("Image Window",screen);
    cv::waitKey(0);
}




// Accessor for camera position
glm::vec3 Camera::getPosition() const{
    return cameraPosition;
}

// Accessor for multi-sampling status
bool Camera::isMultiSamplingEnabled() const{
    return multiSampling;
}

// Get all objects in the scene
const std::vector<std::shared_ptr<Object3D>>& Camera::getObjects() const{
    return objects;
}

// Get Ambient Light
const std::vector<std::shared_ptr<AmbientLight>>& Camera::getAmbientLight() const{
    return ambientLight;
}

// Get all directional lights
const std::vector<std::shared_ptr<DirectionalLight>>& Camera::getDirectionalLights() const{
    return directionalLights;
}

// Get all spotlights
const std::vector<std::shared_ptr<Spotlight>>& Camera::getSpotlights() const{
    return spotlights;
}

void Camera::toString(){
    std::cout << "Camera Position: (" << cameraPosition[0] << ", " << cameraPosition[1] << ", " << cameraPosition[2] << ") ";
    std::cout << std::endl;
    std::cout << "multiSampling: " << multiSampling;
    std::cout << std::endl;
    std::cout << ambientLight[0]->toString();
    std::cout << std::endl;
    for (const auto& element : objects) {
        std::cout << element->toString();
        std::cout << std::endl;
    }
    for (const auto& element : directionalLights) {
        std::cout << element->toString();
        std::cout << std::endl;
    }
    for (const auto& element : spotlights) {
    std::cout << element->toString();
    std::cout << std::endl;
    }
}

// Custom comparator for sorting objects by their Z-coordinate
bool compareByZ(const std::shared_ptr<Object3D>& obj1, const std::shared_ptr<Object3D>& obj2) {
    glm::vec3 parameters1 = obj1 -> getParameters();
    glm::vec3 parameters2 = obj2 -> getParameters();
    return parameters1.z > parameters2.z; 
}

// Function to insert an object in sorted order
void Camera::insertSorted(std::shared_ptr<Object3D> newObject) {
    auto it = std::lower_bound(objects.begin(), objects.end(), newObject, compareByZ);
    objects.insert(it, newObject);
}
