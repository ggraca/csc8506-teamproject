//#pragma once
//#include "Component.h"
//
//
//namespace NCL {
//	namespace CSC8503 {
//		class Particle {
//		public:
//			Particle(Vec3 Position, Vec3 Velocity, float Size) {
//				position = Position;
//				velocity = Velocity;
//				size = Size;
//				lifetime = 0.0f;
//			};
//			Particle() {};
//			~Particle() {};
//
//			float lifetime;
//			Vec3 position;
//			Vec3 velocity;
//			float size;
//		};
//
//		class ParticleSystem : public Component
//		{
//		public:
//			ParticleSystem(float Duration, int MaxParticles, int ParticleSpawnRate, float ParticleLifetime,
//				float ParticleStartSpeed, Vec3 ParticleSpeedDirection, Vec3 ParticleSpeedDeviation,
//				float ParticleStartSize, TextureBase* ParticleTexture);
//			ParticleSystem();
//			~ParticleSystem();
//
//			vector<Particle>& GetParticles() { return particles; }
//			TextureBase* GetParticleTexture() const { return particleTexture; }
//			float GetParticleMaxLifeTime() const { return particleLifetime; }
//
//			void Update(float dt) override;
//
//		protected:
//			float duration;
//			float currentDuration;
//			int maxParticles;
//			int particleSpawnRate;
//			float currentSpawnTime;
//
//			float particleLifetime;
//			float particleStartSpeed;
//			Vec3 particleSpeedDirection;
//			Vec3 particleSpeedDeviation;
//			float particleStartSize;
//			TextureBase* particleTexture;
//
//			vector<Particle> particles;
//
//			void SpawnParticle();
//			void UpdateParticles(float dt);
//			void RemoveOldParticles();
//		};
//	}
//}