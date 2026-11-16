#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################


Condition
	On 	Cycle
	Time 	60

	Event
		EName   RadiusFxScale
		Type	local
		At	Origin
		Bhvr	powers/firecontrol/Inferno.bhvr
		Part	:BurrBloom_Globs_Fast.part
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
		Part1	:BurrBloom_Tick.part
		Part2	:BurrBloom_Dust.part
		Lifespan 30
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