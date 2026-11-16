#
FxInfo

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Start
			Bhvr behaviors/CameraShake01_Strong.bhvr
			Part :Overload_Flash_Big.part
			Part :Overload_Streaks_Windup.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Chest
			Type Start
			Part :Overload_Flash_Initial.part
			Part :Overload_Flash_Big.part
			Part :Overload_Streaks_Big.part
			Part :Overload_Streaks_Big_Alpha.part
			Part :Overload_Streaks_BigB.part
			Part :Overload_Streaks_BigB_Alpha.part
			Part :Overload_Sparks_Big.part
			Part ENEMYFX\Awakened\Malaise_Embers_Big.part
			Part :Overload_Streaks_Big_Highlight.part
			LifeSpan 5
		End

		Event
			At Chest
			Type Start
			Flags	AdoptParentEntity
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				FadeOutLength 1
			End
			Lifespan 1
		End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.1 0.0
		End
		Part	:CoreExplosion_Ground_Scorchmark.part
	#	Part	:CoreExplosion_Ground_Glows.part

	End

	End

	Condition
		On Time
		Time 8

		Event
			At Chest
			Type Start
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				FadeOutLength 1
			End
			Part :Overload_Streaks_Big.part
			Part :Overload_Streaks_Big_Alpha.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 4

		Event
			At Head
			Type Start
			Part :Overload_Shockwave_Big.part
			Part :Overload_Shockwave_Broken.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 7

		Event
			At Chest
			Type Start
			Part :Overload_Smoke_Additive.part
			Part ENEMYFX\Awakened\Overload_Dust.part
			Part ENEMYFX\Awakened\Malaise_Smoke_Big.part
			Part ENEMYFX\Awakened\Malaise_Smoke_Additive.part
			Part ENEMYFX\Awakened\Malaise_Smoke_Wispy.part
			Part :Overload_Electric.part
			LifeSpan 8
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Chest
			Type Start
			CHILDFX	:CHILD_MalaiseDeath_BodyGlow.fx
			LifeSpan 8
		End

	End

End
