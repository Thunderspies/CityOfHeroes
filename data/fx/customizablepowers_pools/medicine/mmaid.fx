#############################################################
## MMAid.fx
#############################################################

FxInfo

Lifespan 160

#############################################################

Condition
	On	Time
	Time	24

	Event
		EName	Pod
		Type	Local
		At	Mystic
		Geom	GEO_WepL_MediPod
		Bhvr	Behaviors\MediPodFadeOut.bhvr
	End
End

Condition
	On	Time
	Time	44	#0

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MWaves01.part
		part2	Powers\Medicine\MCloud01.part
		part3	Powers\Medicine\MGlow.part
		Lifespan 40
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MWaves02.part
		Pmagnet Pod
		Lifespan 44
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