#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	VaporTrail
		Type	Local
		At	 Origin
		Until	AMMO
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part  
		pOther	Target
	End
	
	Event
		Type	Start
		At		WepL
		BhvrOverride
				PYRRotate	0 0 180
		End     
		ChildFX	WEAPONS\DualPistols\Child_SingleShot.fx
		Lifespan 39
	End

	Event
		Type	Start
		At		WepL
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		ChildFX	WEAPONS\DualPistols\Child_ShellEject.fx
		Lifespan 1
	End
	
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	CRYOROUNDS

	Event
		Ename	CryoTrail
		Type	Local
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
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
		Type	PositOnly
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryRound02.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail02.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail03.part
		Part	WEAPONS\DualPistols\BulletTrail_IncendiaryTrail04.part

		pOther	Target
	End

	Event
		EName	IncendiaryMuzzleFlash
		Type	StartPositOnly
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\MuzzleFlash_Incendiary_Flare.part
		Part	WEAPONS\DualPistols\MuzzleFlash_Incendiary_Flash.part
	End

End

Condition
	On 		TriggerBits
	TriggerBits	CHEMICALROUNDS

	Event
		Ename	ChemicalTrail
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotate		90 0 0
		End
		Bhvr	Behaviors\GenericParticleFade.bhvr
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
		Lifespan 1
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