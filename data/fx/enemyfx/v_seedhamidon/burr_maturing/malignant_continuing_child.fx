#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################


Condition
	On 	Cycle
	Time 	120

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	HamidonSeed_Malignant_ContinuingChild_01 100 100 .8
		Sound 	HamidonSeed_Malignant_ContinuingChild_02 100 100 .8
		Sound 	HamidonSeed_Malignant_ContinuingChild_03 100 100 .8
		lifespan 1
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Origin
		Bhvr	powers/firecontrol/Inferno.bhvr
		Part	:Malignant_Globs_Fast.part
		Lifespan 15
	End


	Event
		EName 	Prime1
		Type	StartPositOnly
		At	RadiusFxScale
		bhvr    behaviors/Braz/ExpandRingTo60.bhvr
		BhvrOverride
			ScaleRate		0.2 0.2 0.2
 			EndScale		9 9 9
		End
		Part1	:Malignant_Tick.part
		Part2	:Malignant_Dust.part
		Lifespan 60
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

End