#############################################################
## Child_BulletRain_Right.fx
#############################################################

FxInfo

Lifespan 20

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetOffset1
		At	T_Root
		Type	Start
		BhvrOverride
			StartJitter	5 0 5
		End
	End

	Event
		EName	BulletRainRig1
		At	Origin
		Type	Posit
		BhvrOverride
			Scale		5 5 1
			Stretch		1
		End
		Anim	FX_BulletRain
		LookAt 	TargetOffset1
	End

	Event
		Ename	Bullet1
		At	BulletRainRig1
		AnimPiv	UArmR
		Type	Local
	End

	Event
		Ename	VaporTrail1
		At	Bullet1
		At	Bullet1
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Until	AMMO
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CRYOROUNDS

	Event
		Ename	CryoTracer1
		At	Bullet1
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Lifespan 10
	End

	Event
		Ename	CryoTrail1
		At	Bullet1
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03b.part
	End
End

##############################################################

Condition
	On 		TriggerBits
	TriggerBits	INCENDIARYROUNDS

	Event
		Ename	IncendiaryRounds1
		At	Bullet1
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Lifespan 15
	End

	Event
		Ename	IncendiaryTrail1
		At	Bullet1
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail03_Curved.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail04.part

	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CHEMICALROUNDS

	Event
		Ename	ChemicalTrail1
		At	Bullet1
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01c.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01d.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetOffset2
		At	T_Root
		Type	Start
		BhvrOverride
			StartJitter	10 0 10
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	BulletRainRig2
		At	Origin
		Type	Posit
		BhvrOverride
			Scale		5 5 1
			Stretch		1
		End
		Anim	FX_BulletRain
		LookAt 	TargetOffset2
	End

	Event
		Ename	Bullet2
		At	BulletRainRig2
		AnimPiv	LArmR
		Type	Local
	End

	Event
		Ename	VaporTrail2
		At	Bullet2
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Until	AMMO
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CRYOROUNDS

	Event
		Ename	CryoTracer2
		At	Bullet2
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Lifespan 10
	End

	Event
		Ename	CryoTrail2
		At	Bullet2
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03b.part
	End
End

##############################################################

Condition
	On 		TriggerBits
	TriggerBits	INCENDIARYROUNDS

	Event
		Ename	IncendiaryRounds2
		At	Bullet2
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Lifespan 15
	End

	Event
		Ename	IncendiaryTrail2
		At	Bullet2
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail03_Curved.part
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CHEMICALROUNDS

	Event
		Ename	ChemicalTrail2
		At	Bullet2
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01c.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01d.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetOffset3
		At	T_Root
		Type	Start
		BhvrOverride
			StartJitter	15 0 15
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	BulletRainRig3
		At	Origin
		Type	Posit
		BhvrOverride
			Scale		5 5 1
			Stretch		1
		End
		Anim	FX_BulletRain
		LookAt 	TargetOffset3
	End

	Event
		Ename	Bullet3
		At	BulletRainRig3
		AnimPiv	HandR
		Type	Local
	End

	Event
		Ename	VaporTrail3
		At	Bullet3
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Until	AMMO
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CRYOROUNDS

	Event
		Ename	CryoTracer3
		At	Bullet3
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoRound02.part
		Lifespan 15
	End

	Event
		Ename	CryoTrail3
		At	Bullet3
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail01b.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_CryoTrail03b.part
	End
End

##############################################################

Condition
	On 		TriggerBits
	TriggerBits	INCENDIARYROUNDS

	Event
		Ename	IncendiaryRounds3
		At	Bullet3
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Lifespan 15
	End

	Event
		Ename	IncendiaryTrail3
		At	Bullet3
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail03_Curved.part
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CHEMICALROUNDS

	Event
		Ename	ChemicalTrail3
		At	Bullet3
		Type	Create
		Inherit	Position Rotation
		Update	Position Rotation
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01c.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail01d.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
	End
End

#############################################################

End