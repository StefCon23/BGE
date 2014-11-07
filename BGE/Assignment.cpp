#include "PhysicsGame1.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "Ground.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"

#include "Assignment.h"

using namespace BGE;

Assignment::Assignment(void)
{
}

Assignment::~Assignment(void)
{
}



bool Assignment::Initialise()
{

	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();


	setGravity(glm::vec3(0, -9, 0));

	int i = 0,
		j = 0;
	int k = 5,
		l = 5;

	shared_ptr<PhysicsController> box[5][5];

	for (j = 0; j < l; ++j) {
		for (i = 0; i < k; ++i) {
			box[i][j] = physicsFactory->CreateBox(5, 5, 5, glm::vec3(5 * i, 5 * j, 0), glm::quat());
		}
	}


	if (!Game::Initialise()) {
		return false;
	}



	return true;
}

void BGE::Assignment::Update()
{
	//cyl->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f, 0.0f, 1.0f)));

	Game::Update();
}

void BGE::Assignment::Cleanup()
{
	Game::Cleanup();
}
