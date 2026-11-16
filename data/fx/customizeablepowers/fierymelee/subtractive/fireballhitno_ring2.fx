#############################################################
## FireBallHitNO_RING2.fx
#############################################################

FxInfo
LifeSpan	100

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	time
	Time 	0
	Repeat  5

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\Subtractive\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\Subtractive\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\Subtractive\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\Subtractive\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hook
		Type	Local
		At	root
		Geom	RootToChestAdjustment

	End

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	CustomizeablePowers\FieryMelee\Subtractive\FireBallTargetSteam.part
		Part2	CustomizeablePowers\FieryMelee\Subtractive\FireBallTargetFlame2.part
		Part3	CustomizeablePowers\FieryMelee\Subtractive\FireBallSpark.part
		Sound atomicblast3 80 80 .66

	End


	Event
		Ename	ExplosiveForce
		Type	Start
		At	hips

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	25
		End

		Lifespan	1

	End
	Event
		EName 	Prime1
		Type	Local
		At	chest

		Part1	CustomizeablePowers\FieryMelee\Subtractive\InfernoCare.part
		Part2	CustomizeablePowers\FieryMelee\Subtractive\Inferno.part
		Sound telekinetic3 80 80 .9

	End

	Event
		EName 	Prime2
		Type	Local
		At	chest
		Part1	CustomizeablePowers\FieryMelee\Subtractive\InfernoExplosion.part


	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local
		At	chest
		Part1	CustomizeablePowers\FieryMelee\Subtractive\InfernorockBits.part
		Part2	CustomizeablePowers\FieryMelee\Subtractive\InfernorockBitz.part
		Sound FireExplo 100.0 100.0 .8
	End

End



Condition
	On 	Time
	Time	3
	Repeat  2

	Event
		SoftwareOnly
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX CustomizeablePowers\FireBlast\Subtractive\FireBurstComet.fx
		Lifespan 70
		LifespanJitter 	20
	End

End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End