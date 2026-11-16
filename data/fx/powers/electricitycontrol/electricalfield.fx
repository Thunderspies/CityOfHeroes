#########################################################
## Voltaic Sentinel - Critter FX
########################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part1	:ElectricityControl01a.part
		Part2	:ElectricityControl02a.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityOuterGlow.part
		Part5	:ElectricityArch.part
	End

	Event
		Type	Local
		At	Origin
		Part1	:ElectricityRing.part
		Part2	:ElectricityArchInvert.part
		Part3	:ElectricityInnerGlow2.part
		Part4	:ElectricityOuterRing.part
	End
End

#############################################################

Condition
	On 	Death

	Event
		EName	ALL
		Type	Destroy
	End
End

#############################################################

End