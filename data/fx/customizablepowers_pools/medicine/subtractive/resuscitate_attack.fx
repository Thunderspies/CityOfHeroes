#############################################################
## Resuscitate_Attack.fx
#############################################################

FxInfo
Lifespan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pod
		Type	Local
		At	WepL
		Bhvr	Behaviors\MediPodFadeOut.bhvr
		Geom	GEO_WepL_MediPod
		Sound heal1 60 60 0.7
		Lifespan 150
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
		part1	CustomizablePowers_Pools\Medicine\Subtractive\MGlow.part
		Pmagnet	T_Chest
		Lifespan 40
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
		part1	CustomizablePowers_Pools\Medicine\Subtractive\MReviveFlash01.part
		part2	CustomizablePowers_Pools\Medicine\Subtractive\MReviveFlash02.part
		part3	CustomizablePowers_Pools\Medicine\Subtractive\MReviveGlow.part
		part1	CustomizablePowers_Pools\Medicine\Subtractive\MReviveRays02.part

		Pmagnet T_Chest
		Lifespan 45
	End

	Event
		EName	Prime
		Type	Start
		At	Pod
		Altpiv	1
		Bhvr	Behaviors\MediPodProjectile.bhvr
		part1	CustomizablePowers_Pools\Medicine\Subtractive\MBeamParts01.part
		magnet	T_Chest
		lookat	T_Chest
		Lifespan 45
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	Prime
		Type	Local
		At	Pod
		Altpiv	1
		part1	CustomizablePowers_Pools\Medicine\Subtractive\MReviveBeam01.part
		part2	CustomizablePowers_Pools\Medicine\Subtractive\MReviveBeam02.part
		Pmagnet	T_Chest
		Lifespan 40
	End
End

#############################################################

End