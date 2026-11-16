#############################################################
## DualBlades - Small Big (Child)
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Origin
		Part1	:DualBlades_HitFlash1.part
		Part5	:DualBlades_HitStar1.part
		Part3	:DualBlades_SmallHitEmbers1.part
		Part4	:DualBlades_SmallHitEmbers1.part
		Sound Axe5 80.0 80.0 .8
		Lifespan	5
	End

	Event
		Type 	Local
		At	Origin
		##BHVR	Behaviors/CameraShake_Hit_Subtle01.bhvr
		Part1	Powers/MartialArts/MartialArtsHit.part
		Part2	Powers/MartialArts/MartialArtsHit1.part
		Part3	Powers/MartialArts/MartialArtsHit2.part
		Part4	POWERS\MedievalWeapons\SwordHitRaysLarge.part
	End

	Event
		Type 	Local
		At	Origin
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
	End
	Event
		Type	Local
		At	Chest
		SoundNoRepeat 3
		Sound 	N_Hit_01 100 100 .8
		Sound 	N_Hit_02 100 100 .8
		Sound 	N_Hit_03 100 100 .8
		Sound 	N_Hit_04 100 100 .8
		Sound 	N_Hit_05 100 100 .8
		Sound 	N_Hit_06 100 100 .8
	End


End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplodeUP.fx
	End

	Event
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkUP.fx
	End
End

## HARDWARE ONLY ###########################################################

Condition
	On	Time
	Time	0

	Event
		HardwareOnly
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
	End

	Event
		HardwareOnly
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplodeUP.fx
	End
End

## CHANCE TO GO OFF ###########################################################

Condition
	On	Time
	Time	0
	Chance	0.5

	Event
		EName	Sparkfar
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUTbig.fx
	End
End

Condition
	On	Time
	Time	2
	Chance	0.75

	Event
		EName	Sparkout
		Type 	Start
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
	End
End

#############################################################

End