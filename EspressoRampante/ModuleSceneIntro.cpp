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
	Cube* p_rampaInicial;
	p_rampaInicial = new Cube(50, 2, 50);
	p_rampaInicial->SetPos(0, 0, 0);
	p_rampaInicial->color = Red;
	deathSensor_cube = App->physics->AddBody(*p_rampaInicial, 0.0, true);
	deathSensor_cube->SetTransform(p_rampaInicial->transform.M);
	primitives.PushBack(p_rampaInicial);
	//physBodies.PushBack(deathSensor_cube);

	/*CARRETERAS*/
	CreateElement(new Cube(16, 2, 200), vec3(0, 1, 0), 0, vec3(1, 0, 0)); /*INICIO*/
	CreateElement(new Cube(16, 2, 50), vec3(0, 1, 150), 0, vec3(1, 0, 0)); /*POST RAMPA 1*/
	CreateElement(new Cube(50, 2, 16), vec3(-17, 1, 183), 0, vec3(1, 0, 0)); /*CURVA 1*/
	CreateElement(new Cube(16, 2, 66), vec3(-50, 1, 158), 0, vec3(1, 0, 0)); /*POST CURVA 1*/
	CreateElement(new Cube(50, 2, 16), vec3(-83, 1, 133), 0, vec3(1, 0, 0)); /*CURVA 2*/

	/*RAMPAS*/
	CreateElement(new Cube(16, 2, 5), vec3(0, 2, 100), -25, vec3(1, 0, 0));
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

