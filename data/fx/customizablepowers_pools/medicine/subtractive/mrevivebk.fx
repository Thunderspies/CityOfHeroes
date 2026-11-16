#########################################################
##	chill touch hit
FxInfo

Input
	InpName	Origin
End

Input
	InpName	Target
End

Input
	InpName	wepL
End

Input
	InpName	T_Chest
End

Lifespan 100

##################################


Condition
	On	Time
	Time	0

	Event
		EName	Pod
		Type	Local
		At	WepL
		Geom	GEO_WepL_MediPod
		Bhvr	Behaviors/MediPodFadeOut.bhvr
		Sound heal1 70 70 .8
		Lifespan 30
	End

End

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Start
		At	Pod
		Altpiv	1
		part1	Powers/Medicine/MGlow.part
		Sound heal1 70 70 .8
		Lifespan 10
		Pmagnet T_Chest
	End

End

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Start
		At	Pod
		Altpiv	1
		part1	Powers/Medicine/MReviveFlash01.part
		part2	Powers/Medicine/MReviveFlash02.part
		part3	Powers/Medicine/MReviveGlow.part
		Sound heal1 70 70 .8
		Lifespan 17
		Pmagnet T_Chest
	End

End

Condition
	On	Time
	Time	0

	Event
		EName	Rays
		Type	Start
		At	Pod
		Altpiv	1
		part1	Powers/Medicine/MReviveRays02.part
		Sound heal1 70 70 .8
		Lifespan 10
		Pmagnet T_Chest
	End

End

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type	Start
		At	Pod
		Altpiv	1
		Bhvr	Behaviors/MediPodProjectile.bhvr
		part1	Powers/Medicine/MBeamParts01.part
		Sound heal1 70 70 .8
		Lifespan 15
		magnet T_Chest
	End

End

Condition
	On	Time
	Time	5

	Event
		EName	Prime
		Type	Start
		At	Pod
		Altpiv	1
		part1	Powers/Medicine/MReviveBeam01.part
		part2	Powers/Medicine/MReviveBeam02.part
		Sound heal1 70 70 .8
		Lifespan 8
		Pmagnet T_Chest
	End

End

End