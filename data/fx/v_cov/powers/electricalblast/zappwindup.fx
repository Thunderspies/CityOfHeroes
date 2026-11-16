#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Sound elecwindup_loop 80 80 .76
	End

	Event
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
	End
End

#############################################################

End