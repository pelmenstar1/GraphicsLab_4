#define _USE_MATH_DEFINES

#include "App.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

static void onWindowSizeChanged(GLFWwindow* window, int width, int height);
static void onMouseButtonPressed(GLFWwindow* window, int button, int action, int mods);
static void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);

static const int DEFAULT_WIDTH = 900;
static const int DEFAULT_HEIGHT = 900;

static const uint64_t ROTATION_CHANGE_STEP = 100;
static const uint64_t MIN_ROTATION_DURATION = 100;

static const glm::vec3 _torusSolidColors[] = {
	glm::vec3(1.0, 0.0, 0.0), // red
	glm::vec3(1.0, 0.5, 0.0), // orange
	glm::vec3(1.0, 1.0, 0.0), // yellow
	glm::vec3(0.0, 1.0, 0.0), // green
	glm::vec3(0.0, 0.0, 1.0), // blue
	glm::vec3(0.29, 0, 0.5), // indigo
	glm::vec3(0.58, 0, 0.86) // violet
};

App* App::_currentApp = nullptr;

App::App()
{
	_torus = new Torus(0.5f, 2.0f, 64, 64);
	_torus->setMaterialColor({ 1.0f, 0.0f, 0.0f });

	_axis3D = new Axis3D(5.0f);
	_lightCube = new LightCube();

	_torusRotationAnimator.setLoopDuration(3000);
	_torusColorAnimator.setDuration(1000);

	_torusRotationAnimator.setStartValue(0);
	_torusRotationAnimator.setEndValue((float)(M_PI * 2));

	updateProjectionMatrix(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	setEyePos(glm::vec3(8, 8, 8));
	setLightPos(glm::vec3(0.0, 0.0, 3.5));
}

App::~App()
{
	_currentApp = nullptr;

	glfwDestroyWindow(_window);
	glfwTerminate();

	delete _torus;
	delete _axis3D;
	delete _lightCube;
}

App* App::create()
{
	if (_currentApp != nullptr)
	{
		throw std::exception("App is already started");
	}

	App* app = new App();
	_currentApp = app;

	return app;
}

void App::initGlfw()
{
	if (!glfwInit())
	{
		std::cerr << "Cannot initialize the GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
}

void App::run()
{
	initGlfw();

	_window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Lab 4", NULL, NULL);
	if (!_window)
	{
		std::cout << "Cannot create a window" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetWindowSizeCallback(_window, onWindowSizeChanged);
	glfwSetMouseButtonCallback(_window, onMouseButtonPressed);
	glfwSetKeyCallback(_window, onKeyPressed);

	glfwMakeContextCurrent(_window);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Cannot initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	initGeometry();

	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(0.27f, 0.36f, 0.35f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderFrame();

		glfwSwapInterval(1);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void App::initGeometry()
{
	_torus->initShader();
	_axis3D->initShader();
	_lightCube->initShader();

	_torus->initGeometry();
	_axis3D->initGeometry();
	_lightCube->initGeometry();
}

void App::renderFrame()
{
	if (_isTorusChangingColor)
	{
		glm::vec3 currentTorusColor = _torusColorAnimator.getColor(&_isTorusChangingColor);

		_torus->setMaterialColor(currentTorusColor);
	}

	if (_isTorusRotating)
	{
		float angle = _torusRotationAnimator.getCurrentValue();

		_torus->setRotation(rotation(_torusRotationType, angle));
	}

	if (_showSupportObjects) 
	{
		_axis3D->render(_projMatrix, _viewMatrix);
		_lightCube->render(_projMatrix, _viewMatrix);
	}

	_torus->render(_projMatrix, _viewMatrix);
}

void App::setLightPos(glm::vec3 pos)
{
	_lightCube->setPosition(pos);
	_torus->setLightPos(pos);
}

void App::setEyePos(glm::vec3 pos)
{
	_eye = pos;
	_torus->setEyePos(pos);

	updateViewMatrix();
}

void App::updateViewMatrix()
{
	glm::vec3 center(0, 0, 0);
	glm::vec3 up(0, 0, 1);

	_viewMatrix = glm::lookAt(_eye, center, up);
}

void App::updateProjectionMatrix(uint32_t width, uint32_t height)
{
	float fov = 40 * (float)M_PI / 180.0f;
	float aspect = (float)width / (float)height;

	_projMatrix = glm::perspective(fov, aspect, 1.0f, 50.0f);
}

void App::setViewportSize(uint32_t width, uint32_t height)
{
	if ((width & height) != 0)
	{
		glViewport(0, 0, (int32_t)width, (int32_t)height);

		updateProjectionMatrix(width, height);
	}
}

void App::setTorusColorAnimated(glm::vec3 color)
{
	_isTorusChangingColor = true;

	_torusColorAnimator.setStartColor(_torus->getMaterialColor());
	_torusColorAnimator.setEndColor(color);
	_torusColorAnimator.start();
}

void App::setNextTorusColorAnimated()
{
	size_t n = sizeof(_torusSolidColors) / sizeof(glm::vec3);
	size_t index = (++_torusColorIndex) % n;

	setTorusColorAnimated(_torusSolidColors[index]);
}

void App::switchTorusRotationAxis()
{
	_torusRotationType = (rotation_type)((_torusRotationType + 1) % 2);

	if (_isTorusRotating)
	{
		// Reset the start time for smoother transition.
		_torusRotationAnimator.start();
	}
}

void App::setTorusRotating(bool state)
{
	if (_isTorusRotating == state)
	{
		return;
	}

	_isTorusRotating = state;

	if (state)
	{
		if (_torusRotationTypeOnPause == _torusRotationType)
		{
			_torusRotationAnimator.resume();
		}
		else
		{
			_torusRotationAnimator.start();
		}
	}
	else
	{
		_torusRotationTypeOnPause = _torusRotationType;
		_torusRotationAnimator.pause();
	}
}

void App::switchTorusRotating()
{
	setTorusRotating(!_isTorusRotating);
}

void App::increaseTorusRotationSpeed()
{
	uint64_t newDur = std::max(MIN_ROTATION_DURATION, _torusRotationAnimator.getLoopDuration() - ROTATION_CHANGE_STEP);

	_torusRotationAnimator.setLoopDuration(newDur);
}

void App::decreaseTorusRotationSpeed()
{
	uint64_t newDur = _torusRotationAnimator.getLoopDuration() + ROTATION_CHANGE_STEP;

	_torusRotationAnimator.setLoopDuration(newDur);
}

void App::switchTorusMode()
{
	_torus->setMode((TorusMode)((_torus->getMode() + 1) % TORUS_MODE_COUNT));
}

void App::switchShowSupportObjects() 
{
	_showSupportObjects = !_showSupportObjects;
}

static void onWindowSizeChanged(GLFWwindow* window, int width, int height)
{
	App::getCurrentApp()->setViewportSize(width, height);
}

static void onMouseButtonPressed(GLFWwindow* window, int button, int action, int mods)
{
	App* app = App::getCurrentApp();

	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			app->setNextTorusColorAnimated();
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			app->switchTorusRotating();
		}
	}
}

static void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	App* app = App::getCurrentApp();

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE:
			app->switchTorusRotationAxis();
			break;
		case GLFW_KEY_UP:
			app->increaseTorusRotationSpeed();
			break;
		case GLFW_KEY_DOWN:
			app->decreaseTorusRotationSpeed();
			break;
		case GLFW_KEY_M:
			app->switchTorusMode();
			break;
		case GLFW_KEY_C:
			app->switchShowSupportObjects();
			break;
		}
	}
}