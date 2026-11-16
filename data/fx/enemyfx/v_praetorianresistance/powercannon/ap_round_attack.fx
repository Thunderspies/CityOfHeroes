#############################################################
## Gunfire_ADVAssaultRifle02_Medium.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	WepR
		sound	RES_PowerCannon_FireAPRound 100.0 80.0 .9
		LifeSpan 5
	End
End

#############################################################

Condition
	On 	Time
	Time 	29

	Event
		EName	MuzzleFlash
		Type	Start
		At	C_Chest
	End
End

Condition
	On 	Time
	Time 	29

	Event
		EName	ProjectileLookat
		Type	Create
		Inherit	Position Scale
		Update	None
		At	C_Chest
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	29

	Event
		Ename	ConeOffsets
		Type	Local
		At	ProjectileLookat
		BhvrOverride
			Scale		80 80 80
		End
		Geom	FX_ConeOffsets
	End

	Event
		Ename	TargetOffset
		Type	Local
		At	ConeOffsets
		AltPiv	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerCannon\Child_SingleShot.fx
		Lifespan 4
	End
End

#############################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	PositOnly
		At	C_Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Muzzle_Smoke01.part
		Lifespan 60
	End
End

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	31

	Event
		Ename	Prime
		Type	Start
		At	MuzzleFlash
		BhvrOverride
			TrackRate		10.0
		End
		LookAt	TargetOffset
		Magnet	TargetOffset
	End

	Event
		Ename	Prime2
		Type	Local
		At	Prime
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\BulletTrail_Trail01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\BulletTrail_Trail03.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\BulletTrail_Trail03b.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	31

	Event
		Ename	HelixOffsets
		Type	Local
		At	Prime
		Bhvr	WEAPONS\DualPistols\PiercingRound_Helix.bhvr
		Geom	FX_HelixOffsets
	End

	Event
		Ename	Helix1
		Type	Local
		At	HelixOffsets
		AltPiv	1
		Part	WEAPONS\DualPistols\PiercingRound_HelixTrail01.part
	End

	Event
		Ename	Helix2
		Type	Local
		At	HelixOffsets
		AltPiv	3
		Part	WEAPONS\DualPistols\PiercingRound_HelixTrail01.part
	End

	Event
		Ename	Helix2
		Type	Local
		At	HelixOffsets
		AltPiv	5
		Part	WEAPONS\DualPistols\PiercingRound_HelixTrail01.part
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Prime2
		Type	Destroy
	End

	Event
		EName 	Prime3
		Type	Destroy
	End
End

#############################################################

End