#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

Lifespan 250

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pod
		Type	Local
		At	WepR
		Sound med5 22 22 .57
		Lifespan 250
	End

	Event
		EName	Pod
		Type	Local
		At	WepR
		Bhvr	Behaviors\MediPodFadeOut.bhvr
		Geom	GEO_WepR_MediPod
		Lifespan 200
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:MSelfWaves01.part
		part2	:MSelfWaves02.part
		Pmagnet Neck
		Lifespan 180
	End

	Event
		EName	Glow
		Type	Local
		At	Pod
		Altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:MGlow.part
		Lifespan 180
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	Beams
		Type	Local
		At	Pod
		Altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:MSelfBeam01.part
		Pmagnet Neck
		Lifespan 180
	End
End

#############################################################

End