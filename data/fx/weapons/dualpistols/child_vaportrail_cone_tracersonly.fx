#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 15

#############################################################
Condition
	On		Time
	Time	0

	Event
		Ename	ConeOffsets
		Type	Local                                                            
		At		Origin
		BhvrOverride
			PyrRotateJitter	0 0 180
			Scale		60 60 60
			Alpha 1
		End
		Geom	FX_ConeOffsets 
	End

	Event
		Ename	TargetOffset2
		Type	Start
		At	ConeOffsets
		AltPiv	1
	End 
	
	Event
		Type	PositOnly
		At	TargetOffset2
	End
End

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Prime
		Type	Start
		At	Origin
		BhvrOverride
			StartJitter 		0.1 0.1 0.1
			TrackRate		6.0
		End
		LookAt	TargetOffset2
		Magnet	TargetOffset2
		Lifespan 15
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Lifespan 5
	End

	Event
		Ename	CryoTrail
		Type	Local
		At	Prime
		Bhvr	Behaviors\GenericParticleFade.bhvr
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
		Ename	Incendiary1
		Type	Local
		At	Prime
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Lifespan 5
	End

	Event
		Ename	Incendiary2
		Type	Local
		At	Prime
		Bhvr	Behaviors\GenericParticleFade.bhvr
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
		pOther	TargetOffset2
		Lifespan 10
	End
End

#############################################################

End