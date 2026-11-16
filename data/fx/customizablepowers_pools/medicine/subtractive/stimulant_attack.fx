#############################################################
## Header
#############################################################

FxInfo

#Flags InheritAlpha DontSuppress

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
		Lifespan 65
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
		part2	CustomizablePowers_Pools\Medicine\Subtractive\MCloud01.part
		part1	CustomizablePowers_Pools\Medicine\Subtractive\MTest2.part
		Lifespan 40
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	CustomizablePowers_Pools\Medicine\Subtractive\MStreaks01.part
		Lifespan 25
	End

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	CustomizablePowers_Pools\Medicine\Subtractive\MAidBeam01.part
		part2	CustomizablePowers_Pools\Medicine\Subtractive\MAidBeam02.part
		Lifespan 48
	End
End

#############################################################

End