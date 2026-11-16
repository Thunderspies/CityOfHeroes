#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 60

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	Pod
		Type	Local
		At	WepL
		Bhvr	Behaviors\MediPodFadeIn.bhvr
		Geom	GEO_WepL_MediPod
		Sound med5 70 70 .8
	End
End

Condition
	On	Time
	Time	52

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	:MGlow.part
		Sound xray4 70 70 .7
	End
End

#############################################################

End