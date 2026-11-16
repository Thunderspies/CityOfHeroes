#
FxInfo

	Flags DontSuppress
Lifespan 150

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound CoL_TeleportIn_01 100 100 0.8
		End

		Event
			At Chest
			Type Local
			EName TPAnchor
			Flags	AdoptParentEntity
			CHILDFX	GENERIC\ENTITYFADEIN.FX
			Bhvr WORLD\Bases\MissionArchitect\WarpIn_Fade.bhvr
			BhvrOverride
				FadeInLength 10
			End
		End

		Event
			At TPAnchor
			Type Start
			Part :Beacon_Border.part
			Part :Beacon_Swirls.part
			Part :Beacon_Swirls_Additive.part
			LifeSpan 1
		End

		Event
			At TPAnchor
			Type Start
			EName SpinAnchor
			Part :Beacon_Core.part
			Part :Beacon_Core_Glow.part
		End

	End

	Condition
		On Time
		Time 3

		Event
			At TPAnchor
			Type Start
			Part :Rays_Explode.part
			Part :Streaks_Explode_Detached.part
			Part :Streaks_Explode_Short.part
		End

	End

End
