#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;
	car_gravity = -13.9;
	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = originalMass;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = originalFriction;
	
	car.maxSuspensionForce = 6000.0f;


	car.suspensionStiffness = 20.f;
	car.suspensionCompression = 2.3f;
	car.suspensionDamping = 4.4f;
	

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;
	
	
	vehicle = App->physics->AddVehicle(car);
	
	vehicle->collision_listeners.add(this); // Add this module as listener to callbacks from vehicle
	vehicle->SetPos(0, 5, 10);
	//vehicle->SetRotation(30, { 0, 30, 0 });


	//vehicle->body->setGravity(btVector3(0,-13.9,0));

	vehicle->body->setGravity(btVector3(0, car_gravity, 0));


	/*vehicle->GetTransform(originalTransform);*/
	
	
	
		

	lookForCamera = Cube(0.5, 0.5, 0.5);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		car_gravity -= 1;
		vehicle->body->setGravity(btVector3(0, car_gravity, 0));
		LOG("Nueva gravedad en Y: %f", car_gravity);

	}
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
		car_gravity += 1;
		vehicle->body->setGravity(btVector3(0, car_gravity, 0));
		LOG("Nueva gravedad en Y: %f", car_gravity);

	}


	
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		originalFriction += 0.1f;
	}
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		originalFriction -= 0.1f;
		
	}



	//APLICAR LA FRICCION
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_UP || App->input->GetKey(SDL_SCANCODE_F9) == KEY_UP) {
		int numWheels = vehicle->info.num_wheels;

		// Itera sobre cada rueda y ajusta la fricci�n
		for (int i = 0; i < numWheels; ++i) {
			btWheelInfo& wheel = vehicle->vehicle->getWheelInfo(i);
			wheel.m_frictionSlip = originalFriction;
			vehicle->vehicle->updateWheelTransform(i);
		}
		LOG("Nueva friccion general: %f", originalFriction);
	}


	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_REPEAT) {
		btRigidBody* rbCar = vehicle->vehicle->getRigidBody();
		originalMass += 10;
		
		btVector3 inertia;
		rbCar->getCollisionShape()->calculateLocalInertia(originalMass, inertia);
		rbCar->setMassProps(originalMass, inertia);
		rbCar->updateInertiaTensor();

		int numWheels = vehicle->info.num_wheels;
		for (int i = 0; i < numWheels; ++i) {
			vehicle->vehicle->updateWheelTransform(i);
		}
		LOG("Nueva masa del vehiculo: %f", originalMass);
	}
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_REPEAT && originalMass != 1) {
		btRigidBody* rbCar = vehicle->vehicle->getRigidBody();
		originalMass -= 10;
		if (originalMass <= 0) {
			originalMass = 1;
		}
		btVector3 inertia;
		rbCar->getCollisionShape()->calculateLocalInertia(originalMass, inertia);
		rbCar->setMassProps(originalMass, inertia);
		rbCar->updateInertiaTensor();

		int numWheels = vehicle->info.num_wheels;
		for (int i = 0; i < numWheels; ++i) {
			vehicle->vehicle->updateWheelTransform(i);
		}
		LOG("Nueva masa del vehiculo: %f", originalMass);

	}

	

	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() < -5) {
			brake = BRAKE_POWER;
		}
		else {
			acceleration = MAX_ACCELERATION;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 5) {
			brake = BRAKE_POWER;
		}
		else {
			acceleration = -MAX_ACCELERATION;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		vehicle->SetTransform(vehicle->SetCarRotation(0, { 0, 1, 0 }));
		vehicle->SetPos(0, 5, 10);
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));

		int numWheels = vehicle->info.num_wheels;

		// Itera sobre cada rueda y ajusta la fricci�n
		for (int i = 0; i < numWheels; ++i) {
			btWheelInfo& wheel = vehicle->vehicle->getWheelInfo(i);
			wheel.m_frictionSlip = originalFriction;
			vehicle->vehicle->updateWheelTransform(i);
		}

		for (int i = 0; i < App->scene_intro->ballsCollectionables.Count(); i++) {
			if ((**App->scene_intro->ballsCollectionables.At(i)).picked) {
				(**App->scene_intro->ballsCollectionables.At(i)).picked = false;
				(**App->scene_intro->ballsCollectionables.At(i)).color = Color{ 1,1,0,1 };
			}
		}
		
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();


	vehicle->vehicle->getChassisWorldTransform().getOpenGLMatrix(&lookForCamera.transform);
	btQuaternion q = vehicle->vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(0,7,-15);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	lookForCamera.transform.M[12] += offset.getX();
	lookForCamera.transform.M[13] += offset.getY();
	lookForCamera.transform.M[14] += offset.getZ();
	
	
	App->camera->Position = vec3(lookForCamera.transform[12], lookForCamera.transform[13], lookForCamera.transform[14]);
	App->camera->LookAt(vec3(vehicle->cameraReference.transform[12], vehicle->cameraReference.transform[13], vehicle->cameraReference.transform[14]));
	


	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);
	
	
	
	


	return UPDATE_CONTINUE;
}


void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	// Randomly teleport the sensor cube around 1st quadrant
	if (body1 == App->scene_intro->sensor_cube)	body1->SetPos(20 * rand() / RAND_MAX, 3, 20 * rand() / RAND_MAX);
	if (body2 == App->scene_intro->sensor_cube)	body2->SetPos(20 * rand() / RAND_MAX, 3, 20 * rand() / RAND_MAX);


	if (body2->idType == 5) {
		for (int i = 0; i < App->scene_intro->ballsCollectionables.Count(); i++) {
			if (body2 == (**App->scene_intro->ballsCollectionables.At(i)).phys && !(**App->scene_intro->ballsCollectionables.At(i)).picked) {
				(**App->scene_intro->ballsCollectionables.At(i)).picked = true;
			}
		}
	}





	if (body2 == App->scene_intro->deathSensor_cube) { 
		 
		vehicle->SetTransform(vehicle->SetCarRotation(0, { 0, 1, 0 }));
		vehicle->SetPos(0, 5, 10);
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));	

		int numWheels = vehicle->info.num_wheels;

		// Itera sobre cada rueda y ajusta la fricci�n
		for (int i = 0; i < numWheels; ++i) {
			btWheelInfo& wheel = vehicle->vehicle->getWheelInfo(i);
			wheel.m_frictionSlip = originalFriction;
			vehicle->vehicle->updateWheelTransform(i);
		}


		for (int i = 0; i < App->scene_intro->ballsCollectionables.Count(); i++) {
			if ((**App->scene_intro->ballsCollectionables.At(i)).picked) {
				(**App->scene_intro->ballsCollectionables.At(i)).picked = false;
				(**App->scene_intro->ballsCollectionables.At(i)).color = Color{ 1,1,0,1 };
			}
		}
		
	}

	//Hielo
	if (body2->idType == 2) {
		int numWheels = vehicle->info.num_wheels;

		// Itera sobre cada rueda y ajusta la fricci�n
		for (int i = 0; i < numWheels; ++i) {
			btWheelInfo& wheel = vehicle->vehicle->getWheelInfo(i);
			wheel.m_frictionSlip = 0.8;
			vehicle->vehicle->updateWheelTransform(i);
		}
	}
	if (body2->idType == 3) {
		int numWheels = vehicle->info.num_wheels;

		// Itera sobre cada rueda y ajusta la fricci�n
		for (int i = 0; i < numWheels; ++i) {
			btWheelInfo& wheel = vehicle->vehicle->getWheelInfo(i);
			wheel.m_frictionSlip = originalFriction;
			vehicle->vehicle->updateWheelTransform(i);
		}
	}

}

