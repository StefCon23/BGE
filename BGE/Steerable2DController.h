/*
#pragma once

#include "GameComponent.h"

using namespace std;

namespace BGE
{

	class Steerable2DController :
		public GameComponent
	{
	private:
		void Steerable2DController::CalculateInertiaTensor();
		shared_ptr<Model> model;
	public:
		Steerable2DController(shared_ptr<Model> model);
		~Steerable2DController(void);

		bool Initialise();
		void Update();
		void Draw();

		float mass;
		glm::vec3 velocity, force, acceleration;
		glm::vec3 angularVelocity, angularAcceleration, torque;
		glm::mat3 inertialTensor;

		void AddForce(glm::vec3);
		void AddTorque(glm::vec3);
		void AddForceAtPoint(glm::vec3, glm::vec3);
	};
}


*/