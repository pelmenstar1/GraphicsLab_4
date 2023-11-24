#pragma once

#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Torus.h" 
#include "LightCube.h"
#include "Axis3D.h"
#include "ScalarAnimator.h"
#include "ColorAnimator.h"

class App {
public:

	~App();

	void run();

	void setViewportSize(uint32_t width, uint32_t height);
	void setNextTorusColorAnimated();
	void switchTorusRotating();
	void switchTorusRotationAxis();
	void switchTorusMode();
	void switchShowSupportObjects();

	void increaseTorusRotationSpeed();
	void decreaseTorusRotationSpeed();
	
	static App* getCurrentApp() { return _currentApp; }
	static App* create();
private:
	App();

	static App* _currentApp;

	GLFWwindow* _window = nullptr;
	glm::vec3 _eye;
	glm::mat4 _viewMatrix;
	glm::mat4 _projMatrix;

	Torus* _torus;
	LightCube* _lightCube;
	Axis3D* _axis3D;

	rotation_type _torusRotationType = ROTATION_X;
	rotation_type _torusRotationTypeOnPause = ROTATION_X;
	bool _isTorusRotating = false;
	bool _isTorusChangingColor = false;
	bool _showSupportObjects;

	size_t _torusColorIndex = 0;

	ScalarAnimator _torusRotationAnimator;
	ColorAnimator _torusColorAnimator;

	void setEyePos(glm::vec3 value);
	void setLightPos(glm::vec3 pos);

	void updateViewMatrix();
	void updateProjectionMatrix(uint32_t width, uint32_t height);

	void setTorusColorAnimated(glm::vec3 color);
	void setTorusRotating(bool state);

	void initGeometry();
	void initGlfw();
	void renderFrame();
};