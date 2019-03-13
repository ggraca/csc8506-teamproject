#include "ParticleSystem.h"

using namespace NCL;
using namespace CSC8503;

ParticleSystem::ParticleSystem(float Duration, int MaxParticles, int ParticleSpawnRate, float ParticleLifetime,
	float ParticleStartSpeed, Vector3 ParticleSpeedDirection, Vector3 ParticleSpeedDeviation,
	float ParticleStartSize, TextureBase* ParticleTexture) {

	duration = Duration;
	maxParticles = MaxParticles;
	particleSpawnRate = ParticleSpawnRate;

	particleLifetime = ParticleLifetime;
	particleStartSpeed = ParticleStartSpeed;
	particleSpeedDirection = ParticleSpeedDirection;
	particleSpeedDeviation = ParticleSpeedDeviation;
	particleStartSize = ParticleStartSize;
	particleTexture = ParticleTexture;

	particles.reserve(maxParticles);
}

ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

vector<Vector4>& ParticleSystem::GetParticlePositions() {
	//There is a better way to handle this - don't clear and just update particle positions on the fly
	//Refactor at a later point if its too slow
	particlePositionsSizes.clear();
	for (size_t i = 0; i < particles.size(); i++)
	{
		Vector3 pos = particles[i].position;
		particlePositionsSizes.push_back(Vector4(pos.x, pos.y, pos.z, particles[i].size));
	}
	return particlePositionsSizes;
}

void ParticleSystem::Update(float dt) {
	currentDuration += dt;
	currentSpawnTime += dt;

	while (currentSpawnTime > 1.0f / particleSpawnRate) {
		SpawnParticle();
		currentSpawnTime -= (1.0f / particleSpawnRate);
	}

	UpdateParticles(dt);
	RemoveOldParticles();
}

void ParticleSystem::UpdateParticles(float dt) {
	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i].position += particles[i].velocity;
		particles[i].lifetime += dt;
	}
}

void ParticleSystem::RemoveOldParticles() {
	for (vector<Particle>::const_iterator p = particles.begin(); p < particles.end(); p++)
	{
		if (p->lifetime > particleLifetime) {
			particles.erase(p);
		}
	}
}

void ParticleSystem::SpawnParticle() {
	Vector3 randomVelocityDeviation;
	if (particleSpeedDeviation.x != 0.0f) {
		randomVelocityDeviation.x = -particleSpeedDeviation.x + static_cast <float> (rand())
			/ (static_cast <float> (RAND_MAX / (particleSpeedDeviation.x - -particleSpeedDeviation.x)));
	}
	if (particleSpeedDeviation.y != 0.0f) {
		randomVelocityDeviation.y = -particleSpeedDeviation.y + static_cast <float> (rand())
			/ (static_cast <float> (RAND_MAX / (particleSpeedDeviation.y - -particleSpeedDeviation.y)));
	}
	if (particleSpeedDeviation.z != 0.0f) {
		randomVelocityDeviation.z = -particleSpeedDeviation.z + static_cast <float> (rand())
			/ (static_cast <float> (RAND_MAX / (particleSpeedDeviation.z - -particleSpeedDeviation.z)));
	}

	Vector3 velocity = (particleSpeedDirection + randomVelocityDeviation) * particleStartSpeed;

	particles.emplace_back(Particle(particleLifetime, gameObject->GetTransform().GetWorldPosition(), velocity, particleStartSize));
}