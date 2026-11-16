#############################################################
## Header
#############################################################

FxInfo

#Flags InheritAlpha DontSuppress

Lifespan 100

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pod
		Type	Local
		At	Mystic
		Bhvr	Behaviors\MediPodFadeOut.bhvr
		Geom	GEO_WepL_MediPod
		Lifespan 65
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part2	Powers\Medicine\MCloud01.part
		Lifespan 40
	End
End

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	Ring
#		Type	Local
#		At	Pod
#		Altpiv	1
#		#part1	Powers\Medicine\MWaves02.part
#		Lifespan 50
#	End
#End

Condition
	On	Time
	Time	0

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
	Time	0

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
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MTest2.part
		Lifespan 40
	End
End

#############################################################

End