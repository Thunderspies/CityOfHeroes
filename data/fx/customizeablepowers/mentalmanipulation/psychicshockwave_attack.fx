#############################################################
## PsychicShockwave_Attack.fx
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	rootnode
		Type	local
		At	Root
		Geom	RootToChestAdjustment
	End

	Event
		Type	start
		At	rootnode
		Altpiv	1
		Part	CustomizeablePowers\MentalManipulation\EnergyOuta.part
		Part	CustomizeablePowers\MentalManipulation\EnergyOut2a.part
		LifeSpan 18
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	rootnode
		Type	local
		At	Root
		Part	CustomizeablePowers\MentalManipulation\EnergyOutBurst.part
		Part	CustomizeablePowers\MentalManipulation\EnergyOutBurst2.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	splatHookup
		Type	Local
		At	Root
		BHVR	CustomizeablePowers\MentalManipulation\Spin.bhvr
		Geom	CenterDummy
		LifeSpan 60
	End
End

Condition
	On	cycle
	Time	2

	Event
		Type	start
		At	splatHookup
		Part	CustomizeablePowers\MentalManipulation\EnergyFlash.part
		LifeSpan 4
	End
End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	10

	Event
		Ename	ExplosiveForce
		Type	Posit
		At	Hips
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Lifespan 30
	End
End

#############################################################

End