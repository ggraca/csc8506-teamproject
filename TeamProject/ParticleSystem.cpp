#include "ParticleSystem.h"

using namespace NCL;
using namespace CSC8503;

ParticleSystem::ParticleSystem(float Duration, int MaxParticles, int ParticleSpawnRate, float ParticleLifetime,
	float ParticleStartSpeed, Vector3 ParticleSpeedDirection, Vector3 ParticleSpeedDeviation,
	float ParticleStartSize) {

	duration = Duration;
	maxParticles = MaxParticles;
	particleSpawnRate = ParticleSpawnRate;

	particleLifetime = ParticleLifetime;
	particleStartSpeed = ParticleStartSpeed;
	particleSpeedDirection = ParticleSpeedDirection;
	particleSpeedDeviation = ParticleSpeedDeviation;
	particleStartSize = ParticleStartSize;

	particlePositions.reserve(maxParticles);
	particleVelocities.reserve(maxParticles);
	particleDurations.reserve(maxParticles);
}

ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(float dt) {

}