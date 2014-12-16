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

#include "PhysicsFactory.h"
#include "Game.h" 
#include "Model.h"
#include "dirent.h"
#include "Capsule.h" 

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


	//setGravity(glm::vec3(0, -9, 0));
	setGravity(glm::vec3(0, 0, 0));

	/*
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
	*/

	//shared_ptr<PhysicsController> bird = CreateSeagull(glm::vec3(-10, 30, 0), 5);
	shared_ptr<PhysicsController> bird = CreateSeagull(glm::vec3(-10, 30, 0));
	//CreateBird(glm::vec3(-10, 20, 0));


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

shared_ptr<PhysicsController> Assignment::CreateSeagull(glm::vec3 position, float scale)
{

	/*
	**	Create Seagull
	**
	**	The model was originally supposed to be scalable so sizes are all based on ratios of the scale variable
	**	This makes the code difficult to read and understand
	**	In the later parts I dropped te idea so the model isn't scalable and will not work will values for scale other than 5
	*/


	//	float scale = 5;
	float head_rad = scale / 1.5;
	shared_ptr<PhysicsController> head = physicsFactory->CreateSphere(head_rad, position + glm::vec3(0, scale + head_rad, scale), glm::quat());
	shared_ptr<PhysicsController> body = physicsFactory->CreateSphere(scale, position, glm::quat());
	//btHingeConstraint * head_body = new btHingeConstraint(*head->rigidBody, *body->rigidBody, GLToBtVector(glm::vec3(0, -(head_rad), -(head_rad))), GLToBtVector(glm::vec3(0, scale - 2, scale - 2)), btVector3(1, 0, 0), btVector3(1, 0, 0));	//	body hing bit thing is scale - 2 because head position and scale
	btTransform t1, t2;
	t1.setIdentity();
	t2.setIdentity();
	t1.setOrigin(btVector3(0, -(head_rad), -(head_rad)));
	t2.setOrigin(btVector3(0, scale - 2, scale - 2));
	btFixedConstraint * head_body = new btFixedConstraint(*head->rigidBody, *body->rigidBody, t1, t2);
	dynamicsWorld->addConstraint(head_body);

	/*
	**	wings
	*/

	float shoulder_rad = glm::abs((glm::sqrt(2 * (scale * scale)) - scale) / 2);

	glm::vec3 left_wing_hinge = position + glm::vec3(((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), ((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), 0);
	glm::vec3 right_wing_hinge = position + glm::vec3(-((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), ((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), 0);

	//shared_ptr<PhysicsController> left_shoulder = physicsFactory->CreateBox(scale * 2, scale / 5, scale, left_wing_hinge + glm::vec3((scale + shoulder_rad), 0, 0), glm::quat());
	
	glm::quat shoulder_rot = glm::angleAxis(90.0F, glm::vec3(1, 0, 0));
	shared_ptr<PhysicsController> left_shoulder = physicsFactory->CreateCylinder(shoulder_rad - 0.5, scale / 5, left_wing_hinge, shoulder_rot);
	btHingeConstraint * body_l_shoulder = new btHingeConstraint(*body->rigidBody, *left_shoulder->rigidBody, GLToBtVector(glm::vec3(((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), ((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), 0)), GLToBtVector(glm::vec3(0, 0, 0)), btVector3(0, 0, 0), btVector3(0, 1, 0));
	Game::dynamicsWorld->addConstraint(body_l_shoulder);
	body_l_shoulder->enableAngularMotor(true, 20, 20);

	
	shared_ptr<PhysicsController> left_wing_stick = physicsFactory->CreateBox(scale * 2, scale / 10, scale, left_wing_hinge + glm::vec3((scale)+2, 0, scale / 5), glm::quat());
	btHingeConstraint * l_shoulder_stick = new btHingeConstraint(*left_shoulder->rigidBody, *left_wing_stick->rigidBody, btVector3(shoulder_rad, 0, 0), btVector3(-((scale * 2) - shoulder_rad), 0, 0), btVector3(0, 1, 0), btVector3(0, 0, 1));
	Game::dynamicsWorld->addConstraint(l_shoulder_stick);

	
	

	shared_ptr<PhysicsController> right_shoulder = physicsFactory->CreateCylinder(shoulder_rad - 0.5, scale / 5, right_wing_hinge, shoulder_rot);
	btHingeConstraint * body_r_shoulder = new btHingeConstraint(*body->rigidBody, *right_shoulder->rigidBody, GLToBtVector(glm::vec3(-((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), ((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), 0)), GLToBtVector(glm::vec3(0, 0, 0)), btVector3(0, 0, 0), btVector3(0, 1, 0));
	Game::dynamicsWorld->addConstraint(body_r_shoulder);
	body_r_shoulder->enableAngularMotor(true, -20, 20);

	shared_ptr<PhysicsController> right_wing_stick = physicsFactory->CreateBox(scale * 2, scale / 10, scale, right_wing_hinge + glm::vec3(- (scale / 2), 0, scale / 5), glm::quat());
	btHingeConstraint * r_shoulder_stick = new btHingeConstraint(*right_shoulder->rigidBody, *right_wing_stick->rigidBody, btVector3(-shoulder_rad, 0, 0), btVector3((scale * 2) - shoulder_rad, 0, 0), btVector3(0, 1, 0), btVector3(0, 0, 1));
	Game::dynamicsWorld->addConstraint(r_shoulder_stick);

	


	/*
	**	legs
	*/

	float hip_rad = ((scale + (scale / 2) - (head_rad / 2)) - (scale * (glm::sqrt(3.0) / 2))) / 2;
	shared_ptr<PhysicsController> left_leg = physicsFactory->CreateCylinder((scale / 10), head_rad, position + glm::vec3((scale / 2), -(scale + (scale / 2.5)), 0), glm::quat());
	shared_ptr<PhysicsController> right_leg = physicsFactory->CreateCylinder((scale / 10), head_rad, position + glm::vec3(-(scale / 2), -(scale + (scale / 2.5)), 0), glm::quat());
	
	btTransform t3, t4;
	t3.setIdentity();
	t4.setIdentity();
	t3.setOrigin(btVector3((scale / 2), -((scale * (glm::sqrt(3.0) / 2)) + hip_rad), 0));
	t4.setOrigin(btVector3(0, (head_rad / 2) + hip_rad, 0));
	btFixedConstraint * body_l_leg = new btFixedConstraint(*body->rigidBody, *left_leg->rigidBody, t3, t4);
	dynamicsWorld->addConstraint(body_l_leg);

	btTransform t5, t6;
	t5.setIdentity();
	t6.setIdentity();
	t5.setOrigin(btVector3(-(scale / 2), -((scale * (glm::sqrt(3.0) / 2)) + hip_rad), 0));
	t6.setOrigin(btVector3(0, (head_rad / 2) + hip_rad, 0));
	btFixedConstraint * body_r_leg = new btFixedConstraint(*body->rigidBody, *right_leg->rigidBody, t5, t6);
	dynamicsWorld->addConstraint(body_r_leg);



	return body;
}

shared_ptr<PhysicsController> Assignment::CreateSeagull2(glm::vec3 position)
{

	float scale = 5;
	float head_rad = scale / 1.5;
	shared_ptr<PhysicsController> head = physicsFactory->CreateSphere(head_rad, position + glm::vec3(0, scale + head_rad, scale), glm::quat());
	shared_ptr<PhysicsController> body = physicsFactory->CreateSphere(scale, position, glm::quat());
	
	btTransform t1, t2;
	t1.setIdentity();
	t2.setIdentity();
	t1.setOrigin(btVector3(0, -(head_rad), -(head_rad)));
	t2.setOrigin(btVector3(0, scale - 2, scale - 2));
	btFixedConstraint * head_body = new btFixedConstraint(*head->rigidBody, *body->rigidBody, t1, t2);
	dynamicsWorld->addConstraint(head_body);

	/*
	**	wings
	*/

	float shoulder_rad = glm::abs((glm::sqrt(2 * (scale * scale)) - scale) / 2);

	glm::vec3 left_wing_hinge = position + glm::vec3(((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), ((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), 0);
	glm::vec3 right_wing_hinge = position + glm::vec3(-((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), ((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), 0);
	
	shared_ptr<PhysicsController> left_wing = physicsFactory->CreateBox(scale * 2, scale / 5, scale, left_wing_hinge + glm::vec3((scale + shoulder_rad), 0, 0), glm::quat());
	shared_ptr<PhysicsController> right_wing = physicsFactory->CreateBox(scale * 2, scale / 5, scale, right_wing_hinge + glm::vec3(-(scale + shoulder_rad), 0, 0), glm::quat());
	btHingeConstraint * body_l_wing = new btHingeConstraint(*body->rigidBody, *left_wing->rigidBody, GLToBtVector(glm::vec3(((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), ((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), 0)), GLToBtVector(glm::vec3(-(scale + shoulder_rad), 0, 0)), btVector3(0, 0, 1), btVector3(0, 0, 1));
	btHingeConstraint * body_r_wing = new btHingeConstraint(*body->rigidBody, *right_wing->rigidBody, GLToBtVector(glm::vec3(-((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), ((scale + shoulder_rad) * (1 / glm::sqrt(2.0))), 0)), GLToBtVector(glm::vec3((scale + shoulder_rad), 0, 0)), btVector3(0, 0, 1), btVector3(0, 0, 1));

	body_l_wing->enableAngularMotor(true, -5, 5);
	body_r_wing->enableAngularMotor(true, 5, 5);

	Game::dynamicsWorld->addConstraint(body_l_wing);
	Game::dynamicsWorld->addConstraint(body_r_wing);
	


	/*
	**	legs
	*/

	float hip_rad = ((scale + (scale / 2) - (head_rad / 2)) - (scale * (glm::sqrt(3.0) / 2))) / 2;
	shared_ptr<PhysicsController> left_leg = physicsFactory->CreateCylinder((scale / 10), head_rad, position + glm::vec3((scale / 2), -(scale + (scale / 2.5)), 0), glm::quat());
	shared_ptr<PhysicsController> right_leg = physicsFactory->CreateCylinder((scale / 10), head_rad, position + glm::vec3(-(scale / 2), -(scale + (scale / 2.5)), 0), glm::quat());

	btTransform t3, t4;
	t3.setIdentity();
	t4.setIdentity();
	t3.setOrigin(btVector3((scale / 2), -((scale * (glm::sqrt(3.0) / 2)) + hip_rad), 0));
	t4.setOrigin(btVector3(0, (head_rad / 2) + hip_rad, 0));
	btFixedConstraint * body_l_leg = new btFixedConstraint(*body->rigidBody, *left_leg->rigidBody, t3, t4);
	dynamicsWorld->addConstraint(body_l_leg);

	btTransform t5, t6;
	t5.setIdentity();
	t6.setIdentity();
	t5.setOrigin(btVector3(-(scale / 2), -((scale * (glm::sqrt(3.0) / 2)) + hip_rad), 0));
	t6.setOrigin(btVector3(0, (head_rad / 2) + hip_rad, 0));
	btFixedConstraint * body_r_leg = new btFixedConstraint(*body->rigidBody, *right_leg->rigidBody, t5, t6);
	dynamicsWorld->addConstraint(body_r_leg);



	return body;
}