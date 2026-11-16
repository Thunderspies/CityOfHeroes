#############################################################
## AidBox.fx
#############################################################

FxInfo

Lifespan 160

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pod
		Type	Local
		At	Mystic
		Bhvr	Behaviors\MediPodFadeIn.bhvr
		Part	CustomizablePowers_Pools\Medicine\Subtractive\mglow.part
		Geom	GEO_WepL_MediPod
	End
End

#############################################################

Condition
	On	Time
	Time	45

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MWaves01.part
		part2	Powers\Medicine\MCloud01.part
		part3	Powers\Medicine\MGlow.part
		Sound Power_Analyzer 80 80 .8
		Lifespan 40
	End
End

Condition
	On	Time
	Time	45

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MWaves02.part
		Lifespan 44
		Pmagnet Pod
	End
End

Condition
	On	Time
	Time	45

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MStreaks01.part
		Lifespan 25
	End
End

Condition
	On	Time
	Time	45

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