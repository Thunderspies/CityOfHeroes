#############################################################
## CHILD_PsychicLash_Windup_Small_Offset.fx
#############################################################

FxInfo
Lifespan 45

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	LWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:PsionicShockwave_Smoke.part
		Lifespan 	40
		LifespanJitter	5
	End

	Event
		Type	PositOnly
		At	LWing4
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:PsionicShockwave_Smoke.part
		Lifespan 	40
		LifespanJitter	3
	End
End

#############################################################

End