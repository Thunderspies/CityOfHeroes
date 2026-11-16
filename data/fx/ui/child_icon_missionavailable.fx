#############################################################
## Icon_MissionInProgress.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Origin
		Part	:Star_Green.part
		Part	:Star_Small_Pulse.part
		Lifespan 3
	End

End

Condition
	On	Time
	Time	13

	Event
		Type	Start
		At	Origin
		Part	:Star_Green_Burst.part
		Lifespan 1
	End

End

#############################################################

End