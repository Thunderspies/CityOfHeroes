#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan	50

#########################################################

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\MaceHitMeteors1.bhvr
		Part1	:BurningMeteor.part
		Lifespan	40
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\MaceHitMeteors2.bhvr
		Part1	:BurningMeteor.part
		Lifespan	40
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\MaceHitMeteors3.bhvr
		Part1	:BurningMeteor.part
		Lifespan	40
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\MaceHitMeteors4.bhvr
		Part1	:BurningMeteor.part
		Lifespan	40
	End
End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\MaceHitMeteors5.bhvr
		Part1	:BurningMeteor.part
		Lifespan	40
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	Behaviors\MaceHitMeteors6.bhvr
		Part1	:BurningMeteor.part
		Lifespan	40
	End

End

Condition
	On Time
	Time 0

	Event
		EName	Pow
		Type 	Local
		At	Chest
		BHVR	Behaviors/CameraShake01Subtle.bhvr
		Part1	:MaceFlash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		Sound Axe5 80.0 80.0 .8
	End

	Event
		EName 	Mallet
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MartialArtsHit.part
		Part2	Powers/MartialArts/MartialArtsHit1.part
		Part3	Powers/MartialArts/MartialArtsHit2.part
		Part4	:MaceHitStreaks.part
	End


	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	:MaceHitGlow01Sml.part
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

Condition
	On Time
	Time 2

	Event
		EName	Pow
		Type 	Destroy

	End

End

Condition
	On Time
	Time 0

	Chance	.6
	Repeat	8
	RepeatJitter	2

	Event
		EName	Sparkup
		Type 	Start
		At	hips
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplodeUP.fx

	End

End


Condition
	On Time
	Time 0

	Repeat	5
	RepeatJitter	1

	Event
		EName	Sparkdown
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx

	End

End

Condition
	On Time
	Time 0

	Repeat	7
	RepeatJitter	6

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplode.fx

	End

End

Condition
	On Time
	Time 2

	Repeat	6
	RepeatJitter	4

	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx

	End


End


Condition
	On Time
	Time 0

	Repeat	10
	RepeatJitter	2

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplode.fx
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
	End
End


End

