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

	particles.reserve(maxParticles);
}

ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(float dt) {
	currentDuration += dt;
	currentSpawnTime += dt;

	if (currentSpawnTime > 1.0f / particleSpawnRate) {
		SpawnParticle();
		currentSpawnTime -= (1.0f / particleSpawnRate);
	}

	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i].position += particles[i].velocity;
		particles[i].lifetime += dt;
	}
}

void ParticleSystem::SpawnParticle() {

}