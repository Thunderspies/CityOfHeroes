#########################################################
##	SStreetSteeam01
##
FxInfo

LifeSpan 0

Input
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Origin
		Part1	:Steam_BrokenPipe_Directional.part
		Part	:Steam_BrokenPipe_Directional_Highlights.part
		Part2	:Steam_Pillar.part
		Sound Steam4_Loop 29.0 29.0 .2
	End
End


End