#pragma once

#include "hdtSkyrimMesh.h"
#include "hdtSkinnedMesh\hdtSkinnedMeshWorld.h"
#include "IEventListener.h"
#include "HookEvents.h"

#include <atomic>

namespace hdt
{
	constexpr float RESET_PHYSICS = -10.0f;

	class SkyrimPhysicsWorld : public SkinnedMeshWorld, public IEventListener<FrameEvent>, public IEventListener<ShutdownEvent>
	{
	public:

		static SkyrimPhysicsWorld* get();

		void doUpdate(float delta);
		void updateActiveState();

		virtual void addSkinnedMeshSystem(SkinnedMeshSystem* system) override;
		virtual void removeSkinnedMeshSystem(SkinnedMeshSystem* system) override;
		void removeSystemByNode(void* root);

		void resetSystems();

		virtual void onEvent(const FrameEvent& e) override;
		virtual void onEvent(const ShutdownEvent& e) override;

		inline bool isSuspended() { return m_suspended; }
		inline void suspend(bool loading = false) { m_suspended = true; m_loading = loading; m_resetTimer = true; }
		inline void resume() {
			m_suspended = false;
			if (m_loading)
			{
				resetSystems();
				m_loading = false;
			}
		}

		btVector3 applyTranslationOffset();
		void restoreTranslationOffset(const btVector3&);

		float m_fixedTimeStep = 1 / 60.0f;
		bool m_clampRotations = true;
		bool m_unclampedResets = true;
		float m_unclampedResetAngle = 120.0f;

		bool m_resetTimer = true;
		std::chrono::steady_clock::time_point m_lastFrame;

	private:

		SkyrimPhysicsWorld(void);
		~SkyrimPhysicsWorld(void);

		std::mutex m_lock;

		std::atomic_bool m_suspended;
		std::atomic_bool m_loading;
		float m_accumulatedInterval;
	};
}
