#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

Lifespan 90

#########################################################
####################### AUDIO ###########################



#########################################################
######################	GEO BITS


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Death_01 500 500 .8
	End
End

Condition
	On 	Time
	Time	0

	Event
		Ename	RadiusFxScale
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
	End

	Event
		Ename	SeedChunk01
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_01
		Bhvr	behaviors/GenericParticleFade.bhvr
		bhvrOverride
			InitialVelocity		0.5 0.6 0.4
			InitialVelocityJitter	0.3 0.5 0.2
			Gravity			0.1
			Drag			0.04
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			StartColor		36, 32, 33
			PositionOffset		2.6 1 -1
			FadeOutLength	15
		End
		Lifespan 25
	End

	Event
		Ename	SeedChunk02
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_02
		Bhvr	behaviors/GenericParticleFade.bhvr
		bhvrOverride
			InitialVelocity		0.5 -0.6 0.0
			InitialVelocityJitter	0.3 0.5 0.4
			Gravity			0.08
			Drag			0.04
			Spin			0 0 0
			SpinJitter		0.3 0.05 0.3
			StartColor	36, 32, 33
			PositionOffset	3.5 -1.2 0.4
			FadeOutLength	15
		End
		Lifespan 25
	End

	Event
		Ename	SeedChunk03
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_03
		Bhvr	behaviors/GenericParticleFade.bhvr
		bhvrOverride
			InitialVelocity		0.0 0.8 0.8
			InitialVelocityJitter	0.3 0.5 0.4
			Gravity			0.1
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0.1 0.04 0.3
			StartColor	36, 32, 33
			PositionOffset	0 2.3 4.1
			FadeOutLength	15
		End
		Lifespan 25
	End

	Event
		Ename	SeedChunk04
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_04
		Bhvr	behaviors/GenericParticleFade.bhvr
		bhvrOverride
			InitialVelocity		0.0 1.2 0.1
			InitialVelocityJitter	0.3 0.5 0.3
			Gravity			0.1
			Drag			0.01
			Spin			0 -0.05 0
			SpinJitter		0.1 0.04 0.3
			StartColor	36, 32, 33
			PositionOffset	0 3.2 1.6
			FadeOutLength	15
		End
		Lifespan 25
	End

	Event
		Ename	SeedChunk05
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_05
		Bhvr	behaviors/GenericParticleFade.bhvr
		bhvrOverride
			InitialVelocity		0.0 1.2 -0.3
			InitialVelocityJitter	0.3 0.5 0.5
			Gravity			0.1
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0.1 0.04 0.3
			StartColor	36, 32, 33
			PositionOffset	0 3.5 -1.0
			FadeOutLength	15
		End
		Lifespan 25
	End

	Event
		Ename	SeedChunk06
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_06
		Bhvr	behaviors/GenericParticleFade.bhvr
		bhvrOverride
			InitialVelocity		0.0 0.6 -0.4
			InitialVelocityJitter	0.3 0.5 0.2
			Gravity			0.1
			Drag			0.02
			Spin			0 -0.05 0
			SpinJitter		0.1 0.04 0.3
			StartColor	36, 32, 33
			PositionOffset	0 2.4 -2.1
			FadeOutLength	15
		End
		Lifespan 25
	End

	Event
		Ename	SeedChunk07
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_07
		Bhvr	behaviors/GenericParticleFade.bhvr
		bhvrOverride
			InitialVelocity		-0.5 0.6 0.0
			InitialVelocityJitter	0.3 0.5 0.4
			Gravity			0.08
			Drag			0.01
			Spin			0 0 0
			SpinJitter		0.3 0.05 0.3
			StartColor	36, 32, 33
			PositionOffset	-2.6 1 -1
			FadeOutLength	15
		End
		Lifespan 25
	End

	Event
		Ename	SeedChunk08
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_08
		Bhvr	behaviors/GenericParticleFade.bhvr
		bhvrOverride
			InitialVelocity		-0.5 0.6 0.2
			InitialVelocityJitter	0.3 0.5 0.3
			Gravity			0.13
			Drag			0.04
			Spin			0 -0.05 0
			SpinJitter		0 0.04 0.3
			StartColor	36 32 33
			PositionOffset	-3.5 -1.2 0.4
			FadeOutLength	15
		End
		Lifespan 25
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1
	End
End
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/FadeOut.bhvr
		Flags	AdoptParentEntity
		BhvrOverride
			FadeOutLength	1
			PositionOffset	0 0.4 0.6
		End
		part	:CoreExplosion_Flash_Initial.part
		part	:CoreExplosion_Flash_Big.part
		part	:CoreExplosion_Streaks_Big.part
		part	:CoreExplosion_Streaks_Big_Alpha.part
		Part	:CoreExplosion_Shockwave_Big.part
		Part	:CoreExplosion_Sparks_Big.part
		Part	:CoreExplosion_Splashes.part
		Part	:CoreExplosion_Chunks.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0.4 0.6
		End
		part	:CoreExplosion_Flash_Big.part
		part	:CoreExplosion_Splatter_Hilight.part
		part	:CoreExplosion_Splatter.part
		part	:CoreExplosion_Streaks_BigB.part
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
			InitialVelocity		0.0 0.2 0.0
			InitialVelocityJitter	0.7 0.5 0.7
			Gravity			.02
		End
		Part	:CoreExplosion_InnerSplatter_Purple.part

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
		Part	:CoreExplosion_InnerSplatter_Purple.part
		Lifespan	10
	End
End

#############################################################

End