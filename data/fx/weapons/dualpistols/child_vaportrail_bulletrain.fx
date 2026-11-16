#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 10

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	StartOffsets
		Type	Local
		At	BendMystic
		Geom	FX_ConeOffsets
		Lifespan 6
	End

	Event
		Ename	TargetOffsets
		Type	Local
		At	BendMystic
		BhvrOverride
			PyrRotateJitter		10 15 0
			Scale			30 30 30
		End
		Geom	FX_ConeOffsets
		Lifespan 6
	End

	Event
		Ename	TargetOffset1
		Type	Start
		At	TargetOffsets
		AltPiv	1
	End

	Event
		Ename	TargetOffset2
		Type	Start
		At	TargetOffsets
		AltPiv	2
	End

	Event
		Ename	TargetOffset5
		Type	Start
		At	TargetOffsets
		AltPiv	5
	End
End

#############################################################
## VAPOR TRAIL - 1
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Projectile1
		Type	Start
		At	StartOffsets
		AltPiv	1
		BhvrOverride
			TrackRate		4.0
			StartJitter		0.25 0.25 0.25
		End
		Geom	Bullet01
		LookAt	TargetOffset1
		Magnet	TargetOffset1
		Lifespan 10
	End

	Event
		Ename	VaporTrail
		Type	Local
		At	Projectile1
		Until	AMMO
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
		Lifespan 5
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CRYOROUNDS

	Event
		Ename	CryoTracer1
		Type	Local
		At	Projectile1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Lifespan 5
	End

	Event
		Ename	CryoTrail1
		Type	Local
		At	Projectile1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03b.part
		pOther	TargetOffset2
		Lifespan 10
	End
End

##############################################################

Condition
	On 		TriggerBits
	TriggerBits	INCENDIARYROUNDS

	Event
		Ename	IncendiaryRounds1
		Type	Local
		At	Projectile1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part

		Lifespan 5
	End

	Event
		Ename	IncendiaryTrail1
		Type	PositOnly
		At	Projectile1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
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
		Ename	ChemicalTrail1
		Type	Local
		At	Projectile1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
		pOther	TargetOffset2
		Lifespan 5
	End

	Event
		Ename	ChemicalTrail1
		Type	Start
		At	StartOffsets
		Bhvr	WEAPONS\DualPistols\ChemicalRound_Helix.bhvr
		geom	FX_ChemicalRoundHelix
		Magnet	Projectile1
		LookAt	Projectile1
		Lifespan 1
	End
End

#############################################################
## VAPOR TRAIL - 2
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Projectile2
		Type	Start
		At	StartOffsets
		AltPiv	2
		BhvrOverride
			TrackRate		4.0
			StartJitter		0.25 0.25 0.25
		End
		Geom	Bullet01
		LookAt	TargetOffset2
		Magnet	TargetOffset2
		Lifespan 10
	End

	Event
		Ename	VaporTrail
		Type	Local
		At	Projectile2
		Until	AMMO
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
		Lifespan 5
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CRYOROUNDS

	Event
		Ename	CryoTrail2
		Type	Local
		At	Projectile2
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Lifespan 5
	End

	Event
		Ename	CryoTrail2
		Type	Local
		At	Projectile2
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03b.part
		pOther	TargetOffset2
		Lifespan 10
	End
End

##############################################################

Condition
	On 		TriggerBits
	TriggerBits	INCENDIARYROUNDS

	Event
		Ename	IncendiaryRounds1
		Type	Local
		At	Projectile2
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Lifespan 5
	End

	Event
		Ename	IncendiaryTrail1
		Type	Local
		At	Projectile2
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
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
		Ename	ChemicalTrail2
		Type	Local
		At	Projectile2
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
		pOther	TargetOffset2
		Lifespan 5
	End

	Event
		Ename	ChemicalTrail1
		Type	Start
		At	StartOffsets
		Bhvr	WEAPONS\DualPistols\ChemicalRound_Helix.bhvr
		geom	FX_ChemicalRoundHelix
		Magnet	Projectile2
		LookAt	Projectile2
		Lifespan 1
	End
End

#############################################################
## VAPOR TRAIL - 3
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Projectile3
		Type	Start
		At	StartOffsets
		AltPiv	5
		BhvrOverride
			TrackRate		4.0
			StartJitter		0.25 0.25 0.25
		End
		Geom	Bullet01
		LookAt	TargetOffset5
		Magnet	TargetOffset5
		Lifespan 10
	End

	Event
		Ename	VaporTrail
		Type	Local
		At	Projectile3
		Until	AMMO
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
		Lifespan 5
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CRYOROUNDS

	Event
		Ename	CryoTrail3
		Type	Local
		At	Projectile3
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Lifespan 5
	End

	Event
		Ename	CryoTrail3
		Type	Local
		At	Projectile3
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03b.part
		pOther	TargetOffset2
		Lifespan 10
	End
End

##############################################################

Condition
	On 		TriggerBits
	TriggerBits	INCENDIARYROUNDS

	Event
		Ename	IncendiaryRounds1
		Type	Local
		At	Projectile3
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Lifespan 5
	End

	Event
		Ename	IncendiaryTrail1
		Type	Local
		At	Projectile3
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
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
		Ename	ChemicalTrail3
		Type	Local
		At	Projectile3
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
		pOther	TargetOffset2
		Lifespan 5
	End

	Event
		Ename	ChemicalTrail1
		Type	Start
		At	StartOffsets
		Bhvr	WEAPONS\DualPistols\ChemicalRound_Helix.bhvr
		geom	FX_ChemicalRoundHelix
		Magnet	Projectile3
		LookAt	Projectile3
		Lifespan 1
	End
End

#############################################################

End