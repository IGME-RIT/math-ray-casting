#include "GameObject.h"



void GameObject::update(float dt, glm::mat4 PV)
{
	addForces();

	if (integType == 1)
		eulerIntegration(dt);
	else if (integType == 2)
		RK2Integration(dt);
	else if (integType == 3)
		RK4Integration(dt);

	translation = glm::translate(glm::vec3(position));

	calcTransform();
	MVP = PV * transformation;

	totalForce = acceleration = glm::vec3();


}

void GameObject::addForces()
{
	totalForce += GRAVITY * mass;

	acceleration = totalForce / mass;
}

//Calculates Transformation matrix -> T * R * S
void GameObject::calcTransform()
{
	transformation = translation * rotation * scale;

}

//Updates velocity based off pure Euler integration
void GameObject::eulerIntegration(float dt)
{

	position += velocity * dt;

	//Updating velocity
	velocity += acceleration * dt;

}

//Runge-Kutta Integration 2nd Level
void GameObject::RK2Integration(float dt)
{


	// Get the acceleration at mid of the time step. This is the implementation of the funciton F in RK integrator literature.
	// Since this is a velocity intergrator, It is independant of the displacement. If this were to integrate a spring, the
	// current displacement would also be comuted at the point dt/2 (mid-point)
	velocity += acceleration * dt / 2.0f;

	//Use the velocity at the mid point to compute the displacement during the timestep h
	position += dt * velocity;

	//Change the velocity to the value at the end of the timestep.
	velocity += acceleration * dt / 2.0f;


}

//Runge-Kutta Integration 4th Level
void GameObject::RK4Integration(float dt)
{
	/*
	K1 is the increment based on the slope at the beginning of the interval, uing y (euler's method)
	k2 is the increment based on the slope at the midpoint of the interval, using y + (h/2)k1
	k3 is the increment based on the slope at the midpoint of the interval, using y + (h/2)k2
	k4 is the increment based on the slope at the end of the interval, using y + h*k3

	k1-------------k2-----------------k3----------------k4
	|<--------------------- T -------------------------->|
	*/

	glm::vec3 k1, k2, k3, k4, vel;
	glm::vec3 k;

	vel = velocity;

	//Euler 1
	k = position + velocity * dt;
	vel += acceleration * dt;
	k1 = vel;

	//Euler 2
	k = (position + (dt*k1 / 2.0f)) + vel * dt / 2.0f;
	vel += acceleration * dt / 2.0f;
	k2 = vel;

	//Euler 3
	k = (position + (dt*k2 / 2.0f)) + vel * dt / 2.0f;
	vel += acceleration * dt / 2.0f;
	k3 = vel;

	//Euler 4
	k = (position + (dt*k3)) + vel * dt;
	vel += acceleration * dt;
	k4 = vel;

	//Use the velocity at the mid point to compute the displacement during the timestep h
	position += dt * (k1 + (2.0f * k2) + (2.0f * k3) + k4) / 6.0f;


	//Change the velocity to the value at the end of the timestep.
	velocity += acceleration * dt;

}



void GameObject::setModel(Model *m)
{
	mesh = m;
}


//Return global vertices
std::vector<glm::vec3> GameObject::Vertices()
{

	std::vector<glm::vec3> vertices;

	for (int i = 0; i < mesh->NumVertices(); i++)
	{
		if (mesh->Vertices()[i].position != glm::vec3(0, 0, 0))
		{
			glm::vec4 vertex = transformation * glm::vec4(mesh->Vertices()[i].position.x, mesh->Vertices()[i].position.y, 0, 1);
			vertices.push_back(glm::vec3(vertex.x,vertex.y,0));
		}
	}
	return vertices;

}

//Rotates by x,y and z radians
void GameObject::rotate(glm::vec3 angle)
{
	glm::quat qDelta = glm::quat(angle);

	currentRot *= qDelta;

	rotation = glm::toMat4(currentRot);

	calcTransform();
}

// Sets rotation by x, y and z radians
void GameObject::setRotation(glm::vec3 angle)
{
	currentRot = glm::quat(angle);
	
	rotation = glm::toMat4(currentRot);

	calcTransform();
}

//Initalizing values
GameObject::GameObject(Model* m)
{
	velocity = totalForce = acceleration = position = glm::vec3();

	translation = rotation = scale = transformation = glm::mat4();

	mass = 1.0f;

	mesh = m;

}


GameObject::~GameObject()
{

}
