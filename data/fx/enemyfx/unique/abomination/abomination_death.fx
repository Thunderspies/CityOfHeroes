#
FxInfo

	LifeSpan 500


###########################################################

	Condition
		On Time
		Time 1

		Event
			At Root
			Type Local
			Sound WW_StopServo_01 300 300 0.9
			Sound WW_StopServo_02 300 300 0.9
			Sound WW_StopServo_03 300 300 0.9
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound WW_Death_01 200 200 0.9
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound Maelstrom_DeathExplosion_01 200 200 1
		End

		Event
			At Chest
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				PositionOffset 0 0.4 0.6
				FadeOutLength 10
			End
			Part :Electric_Arc_Sparks_Big.part
			Part :electric_Arc_Straight_Big.part
			Part :electric_Arc_Big.part
			Part :Electric_Arcs_Glow.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Chest
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				PositionOffset 0 0.4 0.6
				FadeOutLength 10
			End
			Part :CoreExplosion_Flash_Initial.part
			Part :CoreExplosion_Streaks_Big.part
			Part :CoreExplosion_Streaks_Big_Alpha.part
			Part :CoreExplosion_Streaks_BigB.part
			Part :CoreExplosion_Streaks_BigB_Alpha.part
			Part :CoreExplosion_Shockwave_Big.part
			Part :CoreExplosion_Sparks_Big.part
			Part ENEMYFX\V_Signature\Maelstrom\CoreExplosion_Flash_Big.part
			Part :CoreExplosion_Embers_Big.part
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Chest
			Type Start
			BhvrOverride
				PositionOffset 0 0.4 0.6
			End
			Part ENEMYFX\V_Signature\Maelstrom\CoreExplosion_Flash_Big.part
			Part :CoreExplosion_Fire.part
			Part :CoreExplosion_Smoke_Additive.part
			Part :CoreExplosion_Smoke_Big.part
			Part :CoreExplosion_Smoke_BigB.part
		End

	End

	Condition
		On Time
		Time 26
		Repeat 1

		Event
			At Chest
			Type Start
			BhvrOverride
				InitialVelocity 0.0 0.5 0.0
				InitialVelocityJitter 0.8 0.5 0.8
				Gravity .02
			End
			Part ENEMYFX\Unique\Abomination\CoreExplosion_Fire_Trail.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 26

		Event
			At Chest
			Type Start
			BhvrOverride
				InitialVelocity 0.0 0.5 0.0
				InitialVelocityJitter 0.8 0.5 0.8
				Gravity .02
			End
			Part ENEMYFX\Unique\Abomination\CoreExplosion_Fire_Trail.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 23
		Repeat 3

		Event
			At Chest
			Type Start
			CEvent ENEMYFX\V_Signature\Maelstrom\CHILD_Sparks_Shrapnel.fx
			ChildFx :CHILD_ElectricalFire_Trail.fx
			CThresh 0.1
			Bhvr Behaviors\FadeOut.bhvr
			BhvrOverride
				StartColor 100 100 100
				InitialVelocity 0.0 0.9 0.0
				InitialVelocityJitter 0.75 0.5 0.75
				Scale 0.2 0.2 0.2
				SpinJitter 0.5 0.5 0.5
				physics 1
				physRadius 0.8
				physGravity 1.5
				physRestitution 0.5
				physSFriction 0.3
				physDFriction 0.5
				FadeOutLength 200
				physDensity 2
			End
			Geom ShrapnelA
			Geom ShrapnelB
			Geom ShrapnelC
			Geom ShrapnelD
			Geom ShrapnelE
			Geom ShrapnelF
			LifeSpan 250
		End

		Event
			At Chest
			Type Start
			CEvent ENEMYFX\V_Signature\Maelstrom\CHILD_Sparks_Shrapnel.fx
			ChildFx :CHILD_ElectricalFire_Trail.fx
			CThresh 0.1
			Bhvr Behaviors\FadeOut.bhvr
			BhvrOverride
				StartColor 100 100 100
				InitialVelocity 0.0 0.9 0.0
				InitialVelocityJitter 0.75 0.5 0.75
				Scale 0.3 0.3 0.3
				SpinJitter 0.5 0.5 0.5
				physics 1
				physRadius 0.8
				physGravity 1.5
				physRestitution 0.5
				physSFriction 0.3
				physDFriction 0.5
				FadeOutLength 200
				physDensity 2
			End
			Geom ShrapnelA
			Geom ShrapnelB
			Geom ShrapnelC
			Geom ShrapnelD
			Geom ShrapnelE
			Geom ShrapnelF
			LifeSpan 250
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			ChildFx :CHILD_Electrical_Continuing.fx
		End

	End

End
