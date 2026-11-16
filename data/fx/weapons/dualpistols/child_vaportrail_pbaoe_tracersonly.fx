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
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail01.part
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
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_ChemicalTrail03b.part
		pOther	Target
		Lifespan 10
	End
End

#############################################################

End