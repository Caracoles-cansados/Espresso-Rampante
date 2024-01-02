#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

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

	/*CARRETERAS*/
	CreateElement(new Cube(16, 2, 200), vec3(0, 1, 0), 0, vec3(1, 0, 0)); /*INICIO*/
	CreateElement(new Cube(16, 2, 50), vec3(0, 1, 150), 0, vec3(1, 0, 0)); /*POST RAMPA 1*/
	CreateElement(new Cube(50, 2, 16), vec3(-17, 1, 183), 0, vec3(1, 0, 0)); /*CURVA 1*/
	CreateElement(new Cube(16, 2, 66), vec3(-50, 1, 158), 0, vec3(1, 0, 0)); /*POST CURVA 1*/
	CreateElement(new Cube(50, 2, 16), vec3(-83, 1, 133), 0, vec3(1, 0, 0)); /*CURVA 2*/
	CreateElement(new Cube(16, 2, 50), vec3(-100, 1, 166), 0, vec3(1, 0, 0)); /*POST CURVA 2*/
	CreateElement(new Cube(80, 2, 16), vec3(-148, 1, 183), 0, vec3(1, 0, 0)); /*CURVA 3*/
	CreateElement(new Cube(16, 2, 50), vec3(-180, 1, 150), 0, vec3(1, 0, 0)); /*POST CURVA 3*/
	CreateElement(new Cube(3, 2, 50), vec3(-180, 1, 100), 0, vec3(1, 0, 0)); /*RECTA ESTRECHA*/
	CreateElement(new Cube(3, 2, 30), vec3(-180, 1, 50), 0, vec3(1, 0, 0)); /*RECTA ESTRECHA 2*/
	CreateElement(new Cube(3, 2, 30), vec3(-180, 1, 10), 0, vec3(1, 0, 0)); /*RECTA ESTRECHA 3*/
	CreateElement(new Cube(16, 2, 80), vec3(-180, 1, -60), 0, vec3(1, 0, 0)); /*RECTA AMPLIA */
	CreateElement(new Cube(196, 2, 5), vec3(-90, 3, -102), 50, vec3(1, 0, 0)); /*RECTA AMPLIA */



	/*RAMPAS*/
	CreateElement(new Cube(16, 2, 10), vec3(0, 2, 100), -25, vec3(1, 0, 0));/*RAMPA 1*/
	CreateElement(new Cube(3, 2, 10), vec3(-180, 3, 75), 25, vec3(1, 0, 0));/*RAMPA ESTRECHA 2*/
	CreateElement(new Cube(3, 2, 10), vec3(-180, 3, 35), 25, vec3(1, 0, 0));/*RAMPA ESTRECHA 3*/
	CreateElement(new Cube(3, 2, 10), vec3(-180, 3, -5), 25, vec3(1, 0, 0));/*RAMPA ESTRECHA 4*/
	//CreateElement(new Cylinder(2,3), vec3(0, 3, 35), 25, vec3(0, 1, 0));


}

void ModuleSceneIntro::RenderElements()
{

	for (int i = 0; i < primitives.Count(); i++) {
		(**primitives.At(i)).Render();
	}

}

Cube* ModuleSceneIntro::CreateElement(Cube* forma, vec3 position, float angle, vec3 axis)
{
	PhysBody3D* pb_rampaInicial;
	
	forma->SetPos(position.x, position.y, position.z);
	pb_rampaInicial = App->physics->AddBody(*forma, 0.0);
	forma->SetRotation(angle, axis);
	pb_rampaInicial->SetTransform(forma->transform.M);
	primitives.PushBack(forma);
	physBodies.PushBack(pb_rampaInicial);


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

