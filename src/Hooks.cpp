#include "Hooks.h"

namespace DualCastingFix
{
	// not the best approach, but this will do

    namespace detail
	{
		bool SetWantCasting(RE::Actor* a_actor, RE::MagicSystem::CastingSource a_source, bool a_enable)
		{
			using func_t = decltype(&SetWantCasting);
			REL::Relocation<func_t> func{ RELOCATION_ID(36925, 37950) };
			return func(a_actor, a_source, a_enable);
		}

		void SetWantDualCasting(RE::Actor* a_actor, bool a_enable)
		{
			using func_t = decltype(&SetWantDualCasting);
			REL::Relocation<func_t> func{ RELOCATION_ID(37814, 38763) };
			return func(a_actor, a_enable);
		}
	}

	struct FinishCastImpl
	{
		static void thunk(RE::ActorMagicCaster* a_this)
		{
			if (const auto actor = a_this->actor) {
			    detail::SetWantCasting(actor, a_this->castingSource, false);
				if (a_this->castingSource == RE::MagicSystem::CastingSource::kLeftHand) {
					detail::SetWantDualCasting(actor, false);
					a_this->flags.reset(RE::ActorMagicCaster::Flags::kDualCasting);
				}
			}
			a_this->flags.reset(RE::ActorMagicCaster::Flags::kSkipCheckCast);
			a_this->costCharged = 0.0f;
		}
		[[maybe_unused]] static inline REL::Relocation<decltype(thunk)> func;

	    static inline std::size_t size = 0x07;
	};

	void Install()
	{
		stl::write_vfunc<RE::ActorMagicCaster, FinishCastImpl>();
	}
}
