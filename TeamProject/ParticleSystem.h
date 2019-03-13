#pragma once
#include "Component.h"

namespace NCL {
	namespace CSC8503 {
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
			int maxParticles;
			int particleSpawnRate;

			float particleLifetime;
			float particleStartSpeed;
			Vector3 particleSpeedDirection;
			Vector3 particleSpeedDeviation;
			float particleStartSize;

		};
	}
}