#pragma once
#include "Component.h"

namespace NCL {
	namespace CSC8503 {
		class Particle {
		public:
			Particle(float Lifetime, Vector3 position, Vector3 velocity, float size) {};
			Particle() {};
			~Particle() {};

			float lifetime;
			Vector3 position;
			Vector3 velocity;
			float size;
		};

		class ParticleSystem : public Component
		{
		public:
			ParticleSystem(float Duration, int MaxParticles, int ParticleSpawnRate, float ParticleLifetime,
				float ParticleStartSpeed, Vector3 ParticleSpeedDirection, Vector3 ParticleSpeedDeviation,
				float ParticleStartSize);
			ParticleSystem();
			~ParticleSystem();

			void Update(float dt) override;

		protected:
			float duration;
			float currentDuration;
			int maxParticles;
			int particleSpawnRate;
			float currentSpawnTime;

			float particleLifetime;
			float particleStartSpeed;
			Vector3 particleSpeedDirection;
			Vector3 particleSpeedDeviation;
			float particleStartSize;

			vector<Particle> particles;
			vector<Vector4> particlePositionsSizes;

			void SpawnParticle();
		};
	}
}