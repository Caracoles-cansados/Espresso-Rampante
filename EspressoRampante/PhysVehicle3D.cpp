#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody3D.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	cameraReference = Cube(1.1f, 1.1f, 1.1f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cameraReference.transform);
	cameraReference.color = Blue;
	btVector3 offset2(0, 2, 3);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	cameraReference.transform.M[12] += offset2.getX();
	cameraReference.transform.M[13] += offset2.getY();
	cameraReference.transform.M[14] += offset2.getZ();

	cameraReference.Render();



	

	



	/*Cube nose2(3, 0.2f, 0.6);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&nose2.transform);
	nose2.color = Blue;
	btQuaternion q_n2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_n2(0, 0.5f, -2);
	offset_n2 = offset_n2.rotate(q_n2.getAxis(), q_n2.getAngle());

	nose2.transform.M[12] += offset_n2.getX();
	nose2.transform.M[13] += offset_n2.getY();
	nose2.transform.M[14] += offset_n2.getZ();
	nose2.Render();*/

	chassis.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

float* PhysVehicle3D::SetCarRotation(float angle, const vec3& u)
{

	transform.rotate(angle, u);
	
	return transform.M;
}



//void PhysVehicle3D::SetRotation(float angle, const vec3& u)
//{
//	transform.rotate(angle, u);
//}
