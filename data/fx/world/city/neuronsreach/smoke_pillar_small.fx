#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Type	StartPositOnly
		At	Root #Origin
		Part1	:Smoke_Pillar_Small.part
		Sound Neutropolis_SmokeStack01_Loop 50 50 .5
	End
End

#############################################################

End