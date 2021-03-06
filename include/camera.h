#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		TURN_LEFT,
		TURN_RIGHT,
		UP,
		DOWN
};

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;


class Camera {

public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;
		
		float MovementSpeed;
		float MouseSensitivity;
		
		Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
//		Camera(float posX = 0.0f, float posY = 0.0f, float posZ= 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f, float yaw = YAW, float pitch = PITCH);

		glm::mat4 GetViewMatrix();

		void ProcessKeyboard(Camera_Movement direction, float deltaTime);

		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
		void TurnCamera(float xoffset, float yoffset, GLboolean constrainPitch = true);

private:
		void updateCameraVectors();

};


#endif
