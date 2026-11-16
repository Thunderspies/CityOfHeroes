#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 15

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Prime
		Type	Local
		At	Origin
		BhvrOverride
			InitialVelocity		5.0 0 0
			Gravity			0.04
		End
		Geom	Bullet01
		Lifespan 15
	End

	Event
		Ename	VaporTrail
		Type	Local
		At	Prime
		Until	AMMO
		BhvrOverride
			PyrRotate		0 0 90
		End
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
		Lifespan 10
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CRYOROUNDS

	Event
		Ename	CryoTrail
		Type	Local
		At	Prime
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Lifespan 5
	End

	Event
		Ename	CryoTrail
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		0 0 90
		End
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03b.part
		pOther	Target
		Lifespan 10
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits 	INCENDIARYROUNDS

	Event
		Ename	IncendiaryTrail
		Type	Local
		At	Prime
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Lifespan 5
	End

	Event
		Ename	IncendiaryTrail
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		0 0 90
		End
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail03.part
		Lifespan 10
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CHEMICALROUNDS

	Event
		Ename	ChemicalTrail
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		0 0 90
		End
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
		pOther	Target
		Lifespan 5
	End

	Event
		Ename	ChemicalTrail
		Type	Start
		At	Origin
		Bhvr	WEAPONS\DualPistols\ChemicalRound_Helix.bhvr
		geom	FX_ChemicalRoundHelix
		Magnet	Prime
		LookAt	Prime
		Lifespan 1
	End
End

#############################################################

End