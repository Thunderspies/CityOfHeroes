#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound CompBeep6_loop 120 120 .46
	End

	Event
		Type	Start
		At	Root
		Sound M1.ogg 120 120 .46
		Lifespan 10
	End
End

Condition
	On	Time
	Time	29

	Event
		Type	Local
		At	WepL
		Sound RiktiTranspond2 70 70 .79
	End
End

Condition
	On	Time
	Time	24

	Event
		EName	Pod
		Type	Local
		At	WepL
		Geom	GEO_WepL_MediPod
		Bhvr	Behaviors/MediPodFadeIn.bhvr
		Sound RiktiTranspond1 70 70 .60
		Part	:mglow.part
	End
End

#############################################################

End