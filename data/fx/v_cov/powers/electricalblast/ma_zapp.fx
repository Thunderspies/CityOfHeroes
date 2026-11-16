#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 180

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	WindupFX
		Type	Local
		At	Root
		ChildFX	:ZappWindup.fx
	End
End

Condition
	On 	Time
	Time 	60

	Event
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 45
	End

	Event
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Lifespan 45
	End
End

Condition
	On 	Time
	Time 	60
	Event
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		Sound lightningbolt2 80 80 .76
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	65

	Event
		Type	Local
		At	HandR
		ChildFx :Child_ElectricBeamCycle.fx
		Lifespan 39
	End
End

#############################################################

End