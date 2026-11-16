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
		Part	:EnergyOuta.part
		Part	:EnergyOut2a.part
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
		Part	:EnergyOutBurst.part
		Part	:EnergyOutBurst2.part
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
		Part	:EnergyFlash.part
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