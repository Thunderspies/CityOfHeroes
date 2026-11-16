#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Prime
		Type	Start
		At	Origin
		BhvrOverride
			TrackRate		7.0
		End
		Geom	Bullet01
		LookAt	T_Head
		Magnet	T_Head
	End

	Event
		Ename	VaporTrail
		Type	Local
		At	Prime
		Until	AMMO
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
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
		While	CRYOROUNDS
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03b.part
		pOther	Target
	End
End

Condition
	On 		TriggerBits
	TriggerBits	INCENDIARYROUNDS

	Event
		Ename	IncendiaryTrail
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail03.part
	End
End

Condition
	On 		TriggerBits
	TriggerBits	CHEMICALROUNDS

	Event
		Ename	ChemicalTrail
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
		pOther	Target
	End

	Event
		Ename	ChemicalTrail
		Type	Start
		At	Origin
		Bhvr	WEAPONS\DualPistols\ChemicalRound_Helix.bhvr
		geom	FX_ChemicalRoundHelix
		Magnet	Prime
		LookAt	Prime
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	ALL
	End
End

#############################################################

End