#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	Pod
		Type	Local
		At	WepL
		Bhvr	Behaviors\MediPodFadeOut.bhvr
		Geom	GEO_WepL_MediPod
		Lifespan 64
	End

	Event
		EName	SOUND
		Type	Local
		At	WepL
		Sound Power_Analyzer 80 80 .8
		Lifespan 30
	End
End

Condition
	On	Time
	Time	25

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MWaves01.part
		part2	Powers\Medicine\MCloud01.part
		part3	Powers\Medicine\MGlow.part
	#	Sound Power_Analyzer 80 80 .8
		Lifespan 40
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MWaves02.part
		Lifespan 44
		Pmagnet Pod
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MStreaks01.part
		Lifespan 25
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MAidBeam01.part
		part2	Powers\Medicine\MAidBeam02.part
		Lifespan 46
	End
End

#############################################################

End