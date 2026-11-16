#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo
Lifespan 500

#############################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX :Jaunt.fx
	End
End


Condition
	On 	Time
	Time 	9


	Event
		Type	Local
		At	Root
		Sound 	Maelstrom_DeathExplosion_01 200 200 1
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/FadeOut.bhvr
		Flags	AdoptParentEntity
		BhvrOverride
			FadeOutLength	10
			PositionOffset	0 0.4 0.6
		End
		part	:CoreExplosion_Flash_Initial.part
		part	:CoreExplosion_Flash_Big.part
		part	:CoreExplosion_Streaks_Big.part
		part	:CoreExplosion_Streaks_Big_Alpha.part
		part	:CoreExplosion_Streaks_BigB.part
		part	:CoreExplosion_Streaks_BigB_Alpha.part
		Part	:CoreExplosion_Shockwave_Big.part
		Part	:CoreExplosion_Sparks_Big.part
		Part	:CoreExplosion_Embers_Big.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	14

	Event
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0.4 0.6
		End
		Part	:CoreExplosion_Streaks_Big_Highlight.part

		part	:CoreExplosion_Flash_Big.part
		part	:CoreExplosion_Fire.part
		Part	:CoreExplosion_Smoke_Additive.part
		Part	:CoreExplosion_Smoke_Big.part
		Part	:CoreExplosion_Smoke_BigB.part
		Part	:CoreExplosion_Sparks_Big_Lingering.part
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.1 0.0
		End
		Part	:CoreExplosion_Ground_Scorchmark.part
		Part	:CoreExplosion_Ground_Glows.part

	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			InitialVelocityJitter	0 0.4 0.6
		End
		Part	:CoreExplosion_Streaks_Big_Highlight.part
	End
End

Condition
	On	Time
	Time	6
	Repeat	1

	Event
		Type	start
		At	Chest
		BhvrOverride
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	0.8 0.5 0.8
			Gravity			.02
		End
		Part	:CoreExplosion_Fire_Trail.part

		Lifespan	10
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	start
		At	Chest
		BhvrOverride
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	0.8 0.5 0.8
			Gravity			.02
		End
		Part	:CoreExplosion_Fire_Trail.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	3
	Repeat	12

	Event
		Type	start
		At	Chest


		Bhvr	Behaviors\FadeOut.bhvr
		BhvrOverride
			StartColor		100 100 100
			InitialVelocity		0.0 0.9 0.0
			InitialVelocityJitter	0.75 0.5 0.75
			Scale			0.5 0.5 0.5
			SpinJitter		0.5 0.5 0.5
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		200
			PhysDensity		2
		End
		Geom	ShrapnelA
		Geom	ShrapnelB
		Geom	ShrapnelC
		Geom	ShrapnelD
		Geom	ShrapnelE
		Geom	ShrapnelF
		ChildFX	:CHILD_Fire_Trail.fx
		CEvent	:CHILD_Sparks_Shrapnel.fx
		CThresh 0.1
		Lifespan	250
	End
End

#############################################################

End