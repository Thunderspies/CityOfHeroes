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
		part1	:MWaves01.part
		part2	:MCloud01.part
		part3	:MGlow.part
		Lifespan 40
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	:MWaves02.part
		Lifespan 44
		Pmagnet Pod
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	:MStreaks01.part
		Lifespan 25
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	:MAidBeam01.part
		part2	:MAidBeam02.part
		Lifespan 46
	End
End

#############################################################

End