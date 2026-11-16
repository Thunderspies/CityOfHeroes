#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On	Time
	Time	5

	Event
		EName	Pod
		Type	Local
		At	Mystic
		Bhvr	Behaviors\MediPodFadeIn.bhvr
		Geom	GEO_WepL_MediPod
		Lifespan 225
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	WepL
		Sound	med5 70 70 0.8
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MGlow.part
	End
End

#############################################################

Condition
	On	Time
	Time	116

	Event
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MGlow.part
		Pmagnet	T_Chest
		Sound heal1 60 60 0.7
		Lifespan 40
	End

	Event
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MReviveFlash01.part
		part2	Powers\Medicine\MReviveFlash02.part
		part3	Powers\Medicine\MReviveGlow.part
		part1	Powers\Medicine\MReviveRays02.part
		Pmagnet T_Chest
		Lifespan 45
	End
End

Condition
	On	Time
	Time	116

	Event
		Type	Start
		At	Pod
		Altpiv	1
		Bhvr	Behaviors\MediPodProjectile.bhvr
		part1	Powers\Medicine\MBeamParts01.part
		magnet	T_Chest
		lookat	T_Chest
		Lifespan 45
	End
End

Condition
	On	Time
	Time	121

	Event
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers\Medicine\MReviveBeam01.part
		part2	Powers\Medicine\MReviveBeam02.part
		Pmagnet	T_Chest
		Lifespan 40
	End
End


#############################################################

End