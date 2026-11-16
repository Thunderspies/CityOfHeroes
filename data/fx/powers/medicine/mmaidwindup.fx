#############################################################
## MMAidWindup.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On	Time
	Time	48

	Event
		EName	Pod
		Type	Local
		At	Mystic
		Bhvr	Behaviors\MediPodFadeIn.bhvr
		Geom	GEO_WepL_MediPod
		Sound med5 70 70 .8
	End
End

Condition
	On	Time
	Time	104

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MGlow.part
		Sound xray4 70 70 .8
	End
End

#############################################################

End