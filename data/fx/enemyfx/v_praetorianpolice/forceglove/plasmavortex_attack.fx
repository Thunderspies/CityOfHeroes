#########################################################
## Energy Melee - Whirling Hands
#########################################################

FxInfo

lifespan 60

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Sound PPD_ForceVortex_Attack_01 100 100 1
	End
End

## HANDS #######################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LArmR
		ChildFX	ENEMYFX\V_PraetorianPolice\ForceGlove\Child_ForceGlove_Attack_Right.fx
		Lifespan 30
	End

	Event
		Type	Local
		At	WepR
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaVortex_AttackTrail01.part
		Lifespan 30
	End
End

## GROUND FX #######################################################

Condition
	On 	Time
	Time 	36

	Event
		Type	start
		At	Mystic
				Sound PPD_ForceVortex_Hit_01 100 100 1
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaVortex_HitFlash01.part
		##Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaVortex_HitFlash02.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaVortex_HitRingFlat.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaVortex_HitRingThinFlat.part
		LifeSpan 5
	End
End

## PHYSICS FORCES #######################################################

Condition
	On 	Time
	Time 	36

	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		800
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#########################################################

End