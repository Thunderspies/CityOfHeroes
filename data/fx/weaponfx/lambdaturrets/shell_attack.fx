#############################################################
## Turret_MuzzleFlash.fx
#############################################################

FxInfo
LifeSpan 50

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	MuzzleAnchor
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 2.5 51.0
			PyrRotate	90 270 0
		End
	Sound LambdaTurret_Shell_Attack_01 300 300 .8
	#	Lifespan	30
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	MuzzleAnchor
		Part	:Flash_Beam.part
		Part	:Flash_Beam_Highlight.part
		Part	:Flash_Beam_Smoke.part
		Part	:Flash_Beam_light.part
		Lifespan	12
	End

	Event
		Type	Local
		At	MuzzleAnchor
		Part	:Flash_Circle.part
		Part	:Flash_Circle_Large.part
		Part	:Flash_Circle_Fire.part
		Lifespan	2
	End

#############################################################

	Event

		EName 	Prime
		Type	Start
		At	MuzzleAnchor
		Bhvr	behaviors\MorterLobbprojectile2.bhvr
		BhvrOverride
			TrackRate	5
		End
		Part	:Grenade_Glow_Chemical.part
		Part	:Grenade_Globs_Chemical.part
		Part	:Grenade_Trail_Smoke_Chemical.part
		Part	:Grenade_Trail_Smoke_Chemical_Full.part
		Geom	grenade
		Magnet	T_Root
		LookAt	T_Root
	End
End

#############################################################

Condition
	On	PrimeHit
	Event
		Type	Local
		At	T_Root
		ChildFX	:Shell_Dud.fx
	End

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End