#############################################################
## Medic_Stimulant.fx
#############################################################

FxInfo

Lifespan 120

#############################################################

Condition
	On	Time
	Time	24

	Event
		EName	Pod
		Type	Local
		At	WepL
		Geom	GEO_WepL_MediPod
		Bhvr	Behaviors\MediPodFadeOut.bhvr
	End
End

Condition
	On	Time
	Time	44

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part2	Powers\Medicine\MCloud01.part
		Lifespan 40
	End
End

Condition
	On	Time
	Time	44

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		Lifespan 50
	End
End

Condition
	On	Time
	Time	44

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
	Time	44

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MAidBeam01.part
		part2	Powers\Medicine\MAidBeam02.part
		Lifespan 48
	End
End

Condition
	On	Time
	Time	44

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\Stimulent.part
		Lifespan 40
	End
End

#############################################################

End