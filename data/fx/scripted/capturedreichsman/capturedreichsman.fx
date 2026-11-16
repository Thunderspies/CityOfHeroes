#############################################################
## CapturedReichsman.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	1

	Event
		EName	DarkLight
		Type	SetLight
		BHVR	POWERS\ItemsAndTempPowers\ShadowyPresence\LightPulse_Dark.bhvr
		Lifespan 90
	End
End

## BODY ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:BodyGlow_Rays1.part
		Part1	:BodyGlow_Rays2.part
		pOther	Chest
	End
End

#############################################################

End