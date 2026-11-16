#############################################################
## PistolHit2.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Input
	Inpname Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter	0.5 1.0 0.5
		End
		Part1	:Cryonic_Hit_Mist_Large.part
		Part2	:Cryonic_Hit_Flash_Large.part
		Part	:Cryonic_Hit_IcePieces.part
		Part	:Cryonic_Hit_Shockwave_Large.part
		Part	:Cryonic_Hit_Mist_Bright.part

		LifeSpan 10
	Lifespan 10

	End
End

Condition
	On	Time
	Time	0
	Repeat	2
	RepeatJitter 1

	Event
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	tintable_FX_IceWall_01
		geom	tintable_FX_IceWall_02
		geom	tintable_FX_IceWall_03
		geom	tintable_FX_IceWall_04
		geom	tintable_FX_IceWall_05
		geom	tintable_FX_IceWall_06
		geom	tintable_FX_IceWall_07
		geom	tintable_FX_IceWall_08
		geom	tintable_FX_IceWall_09
		geom	tintable_FX_IceWall_10
		geom	tintable_FX_IceWall_11
		geom	tintable_FX_IceWall_12
		CThresh 	0.000001
		LifeSpan 	150
		LifeSpanJitter 	50
	End
End



Condition
	On	Cycle
	Time	6

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter	0.5 1.0 0.5
		End
		Part1	:Cryonic_Hit_Mist.part
		Part2	:Cryonic_Hit_Flash.part
		Part	:Cryonic_Hit_IcePieces.part
		Part	:Cryonic_Hit_Streaks.part
		Part	:Cryonic_Hit_Shockwave.part
		LifeSpan 10
	Lifespan 10

	End
End

#############################################################

End