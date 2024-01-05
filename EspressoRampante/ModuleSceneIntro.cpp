#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include <gl/GL.h>
#include "glut/glut.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// Create sensor cube (will trigger with car)
	sensor_cube = App->physics->AddBody(Cube(5, 5, 5), 0.0);
	sensor_cube->SetAsSensor(true);
	sensor_cube->SetPos(0, 3, 0);


	
	this->CreateElements();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	
	this->RenderElements();

	
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateElements()
{
	Cube* cuboRojo;
	cuboRojo = new Cube(550, 2, 550);
	cuboRojo->SetPos(0, 0, 0);
	cuboRojo->color = Red;
	deathSensor_cube = App->physics->AddBody(*cuboRojo, 0.0, true);
	deathSensor_cube->SetTransform(cuboRojo->transform.M);
	primitives.PushBack(cuboRojo);
	//physBodies.PushBack(deathSensor_cube);

	Cube* icedCube;

	/*CARRETERAS*/
	CreateElement(new Cube(16, 2, 200), vec3(0, 1, 0), 0, vec3(1, 0, 0)); /*INICIO*/
	CreateElement(new Cube(16, 15, 40), vec3(0, 1, 140), 0, vec3(1, 0, 0)); /*POST RAMPA 1*/
	CreateElement(new Cube(50, 2, 22), vec3(-17, 1, 180), 0, vec3(1, 0, 0)); /*CURVA 1*/
	CreateElement(new Cube(16, 2, 66), vec3(-50, 1, 158), 0, vec3(1, 0, 0)); /*POST CURVA 1*/
	CreateElement(new Cube(50, 2, 16), vec3(-83, 1, 133), 0, vec3(1, 0, 0))->color = Blue; /*CURVA 2*/
	//Sensor para cambiar a la friccion de hielo
	icedCube = CreateElement(new Cube(50, 2, 16), vec3(-83, 3, 133), 0, vec3(1, 0, 0));
	icedCube->renderPrimitive = false;
	icedCube->phys->SetAsSensor(true);
	icedCube->phys->idType = 2;

	//Sensor para volver a la friccion original
	icedCube = CreateElement(new Cube(5, 2, 16), vec3(-53, 3, 133), 0, vec3(1, 0, 0));
	icedCube->renderPrimitive = false;
	icedCube->phys->SetAsSensor(true);
	icedCube->phys->idType = 3;
	

	
	CreateElement(new Cube(16, 2, 50), vec3(-100, 1, 166), 0, vec3(1, 0, 0))->color = Blue; /*POST CURVA 2*/
	//Sensor para cambiar a la friccion de hielo
	icedCube = CreateElement(new Cube(16, 2, 50), vec3(-100, 3, 166), 0, vec3(1, 0, 0));
	icedCube->renderPrimitive = false;
	icedCube->phys->SetAsSensor(true);
	icedCube->phys->idType = 2;

	CreateElement(new Cube(80, 2, 16), vec3(-148, 1, 183), 0, vec3(1, 0, 0))->color = Blue; /*CURVA 3*/
	//Sensor para cambiar a la friccion de hielo
	icedCube = CreateElement(new Cube(80, 2, 16), vec3(-148, 3, 183), 0, vec3(1, 0, 0));
	icedCube->renderPrimitive = false;
	icedCube->phys->SetAsSensor(true);
	icedCube->phys->idType = 2;


	//Sensor para volver a la friccion original
	icedCube = CreateElement(new Cube(16, 2, 5), vec3(-180, 3, 170), 0, vec3(1, 0, 0));
	icedCube->renderPrimitive = false;
	icedCube->phys->SetAsSensor(true);
	icedCube->phys->idType = 3;
	

	
	CreateElement(new Cube(16, 2, 50), vec3(-180, 1, 150), 0, vec3(1, 0, 0)); /*POST CURVA 3*/
	CreateElement(new Cube(3, 2, 50), vec3(-180, 1, 100), 0, vec3(1, 0, 0)); /*RECTA ESTRECHA*/
	CreateElement(new Cube(3, 2, 30), vec3(-180, 1, 50), 0, vec3(1, 0, 0)); /*RECTA ESTRECHA 2*/
	CreateElement(new Cube(3, 2, 30), vec3(-180, 1, 10), 0, vec3(1, 0, 0)); /*RECTA ESTRECHA 3*/
	CreateElement(new Cube(16, 2, 83), vec3(-180, 1, -60), 0, vec3(1, 0, 0)); /*RECTA AMPLIA */
	CreateElement(new Cube(196, 2, 5), vec3(-90, 3, -102), 50, vec3(1, 0, 0)); /*RECTA AMPLIA */



	/*RAMPAS*/	
	CreateElement(new Cube(16, 2, 10), vec3(0, 2, 100), -25, vec3(1, 0, 0));/*RAMPA 1*/
	CreateElement(new Cube(16, 2, 50), vec3(0, -3, 182), 25, vec3(1, 0, 0));/*RAMPA BAJADA*/
	CreateElement(new Cube(3, 2, 10), vec3(-180, 3, 75), 25, vec3(1, 0, 0));/*RAMPA ESTRECHA 2*/
	CreateElement(new Cube(3, 2, 10), vec3(-180, 3, 35), 25, vec3(1, 0, 0));/*RAMPA ESTRECHA 3*/
	CreateElement(new Cube(3, 2, 10), vec3(-180, 3, -5), 25, vec3(1, 0, 0));/*RAMPA ESTRECHA 4*/
	//CreateElement(new Cylinder(2,3), vec3(0, 3, 35), 25, vec3(0, 1, 0));


	CreateCollectionable(btVector3(0, 3, 30)); //Moneda
	CreateCollectionable(btVector3(0, 3, 20)); //Moneda
	CreateCollectionable(btVector3(0, 3, 10)); //Moneda
	

	CreateRope(btVector3(0, 10, 40));
	CreateRope(btVector3(3, 10, 40));
	CreateRope(btVector3(-3, 10, 40));


	

}

void ModuleSceneIntro::CreateRope(btVector3 startPosition) {
	//btVector3 startPosition(0, 10, 0);  // Posición inicial de la cuerda

	int NumSpheres = 5;
	btScalar SphereRadius = 1.0;
	btScalar SphereMass = 1.0;

	btRigidBody* previousSphere = nullptr;

	for (int i = 0; i < NumSpheres; ++i) {
		// Crear la esfera
		btCollisionShape* sphereShape = new btSphereShape(SphereRadius);
		
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(startPosition);

		// Configurar el cuerpo rígido
		btVector3 localInertia(0, 0, 0);
		sphereShape->calculateLocalInertia(SphereMass, localInertia);

		btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(SphereMass, motionState, sphereShape, localInertia);
		btRigidBody* sphereBody = new btRigidBody(rbInfo);
		
		// Añadir la esfera al mundo de física
		App->physics->world->addRigidBody(sphereBody);
		

		// Añadir restricciones P2P entre esferas adyacentes
		if (previousSphere) {
			btVector3 pivotA(0, SphereRadius, 0);  // Punto de conexión en la esfera anterior
			btVector3 pivotB(0, -SphereRadius, 0); // Punto de conexión en la esfera actual

			btPoint2PointConstraint* p2pConstraint = new btPoint2PointConstraint(*previousSphere, *sphereBody, pivotA, pivotB);
			App->physics->world->addConstraint(p2pConstraint, true);
		}

		// Hacer la primera esfera estática
		if (i == 0) {
			sphereBody->setMassProps(0.0, localInertia);
			sphereBody->setCollisionFlags(sphereBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		}

		// Actualizar la posición inicial para la próxima esfera
		startPosition += btVector3(0, -2 * SphereRadius, 0);

		// Actualizar la referencia a la esfera anterior
		previousSphere = sphereBody;
	}
}

void ModuleSceneIntro::CreateCollectionable(btVector3 position)
{


	Cube* s = new Cube(2,2,2);
	PhysBody3D* physBody;
	
	s->SetPos(position.x(), position.y(), position.z());
	physBody = App->physics->AddBody(*s, 0.0);
	physBody->idType = 5;
	physBody->SetTransform(s->transform.M);
	physBody->SetAsSensor(true);
	s->phys = physBody;
	s->picked = false;
	s->color = Color{1,1,0,1};

	ballsCollectionables.PushBack(s);
	physBodies.PushBack(physBody);


}

void ModuleSceneIntro::RenderElements()
{

	for (int i = 0; i < primitives.Count(); i++) {
		if ((**primitives.At(i)).renderPrimitive) {
			(**primitives.At(i)).Render();
		}
	}

	for (int i = 0; i < ballsCollectionables.Count(); i++) {
		if ((**ballsCollectionables.At(i)).picked) {
			(**ballsCollectionables.At(i)).color = Green;
			
		}
		(**ballsCollectionables.At(i)).Render();
		
	}


	// Renderizar esferas de la cuerda
	// Renderizar solo las esferas de la cuerda
	for (int i = 0; i < App->physics->world->getNumCollisionObjects(); ++i) {
		btCollisionObject* obj = App->physics->world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		if (body && body->getMotionState()) {
			// Verificar si es una esfera
			if (body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {
				btTransform transform;
				body->getMotionState()->getWorldTransform(transform);

				btScalar mat[16];
				transform.getOpenGLMatrix(mat);

				glPushMatrix();
				glMultMatrixf(mat);
				glColor4f(1, 1, 1, 1);
				glutSolidSphere(1.0, 20, 20);  // Ajusta los parámetros según sea necesario
				glPopMatrix();
			}
		}
	}

}

Cube* ModuleSceneIntro::CreateElement(Cube* forma, vec3 position, float angle, vec3 axis)
{
	PhysBody3D* physBody;
	
	forma->SetPos(position.x, position.y, position.z);
	physBody = App->physics->AddBody(*forma, 0.0);
	forma->SetRotation(angle, axis);
	physBody->SetTransform(forma->transform.M);

	forma->phys = physBody;
	

	primitives.PushBack(forma);
	physBodies.PushBack(physBody);


	return forma;
}

void ModuleSceneIntro::CreateElement(Cylinder* forma, vec3 position, float angle, vec3 axis)
{
	PhysBody3D* pb_rampaInicial;

	
	forma->SetPos(position.x, position.y, position.z);
	pb_rampaInicial = App->physics->AddBody(*forma, 0.0);
	forma->SetRotation(angle, axis);
	pb_rampaInicial->SetTransform(forma->transform.M);
	primitives.PushBack(forma);
	physBodies.PushBack(pb_rampaInicial);
}

