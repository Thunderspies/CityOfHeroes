#############################################################
## Resuscitate_Windup.fx
#############################################################

FxInfo
Lifespan 120

#############################################################

Condition
	On	Time
	Time	5

	Event
		EName	Pod
		Type	Local
		At	WepL
		Bhvr	Behaviors\MediPodFadeIn.bhvr
		Geom	GEO_WepL_MediPod
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	WepL
		Sound med5 70 70 .8
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	:MGlow.part
	End
End

#############################################################

End