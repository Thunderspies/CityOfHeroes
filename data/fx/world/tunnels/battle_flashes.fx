#############################################################
## Smoke_StormCloud.fx
#############################################################

FxInfo
Lifespan 90

#############################################################

Input
	InpName	Origin
End

#############################################################
Condition
	On	TIme
	Time	0

	Event
		EName	TempAnchor
		Type	Start
		At	Origin
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.5

	Event
		Type	Start
		At	TempAnchor #Origin
		Part	:Flashes_Cloud.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.35
	Event
		Type	Start
		At	TempAnchor #Origin
		Part	:Flashes_Cloud.part
		Lifespan 1
	End
End