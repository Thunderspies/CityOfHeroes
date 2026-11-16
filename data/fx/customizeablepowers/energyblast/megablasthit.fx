#############################################################
## MegaBlastHit.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Ring1
		Type	Start
		At	chest
		Bhvr	CustomizeablePowers\EnergyBlast\SpinHit1.bhvr
		Part1	CustomizeablePowers\EnergyBlast\EnergyShockRing.part
		Sound Megablasthit2 80 80 .95
		Lifespan 30
	End

	Event
		EName 	hit2
		Type	local
		At	chest
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsHit.part
		Part2	CustomizeablePowers\EnergyBlast\DarkEnergyBallsHit.part
		Part3	CustomizeablePowers\EnergyBlast\SolidEnergyBallsHit.part
		Part4	CustomizeablePowers\EnergyBlast\BlastHitRings.part
		Lifespan 30
	End

	Event
		EName 	Burst
		Type	local
		At	chest
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsBurst.part
		Part2	CustomizeablePowers\EnergyBlast\EnergyBallsBurst2.part
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Chest
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		15
			PhysForcePower		100
			PhysForcePowerJitter	25
		End
		Lifespan 10
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		600
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Ename	Ring1
		Type	Start
		At	chest
		bhvr	CustomizeablePowers\EnergyBlast\SpinHit2.bhvr
		Part1	CustomizeablePowers\EnergyBlast\EnergyShockRing.part
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Ename	Ring1
		Type	Start
		At	chest
		bhvr	CustomizeablePowers\EnergyBlast\SpinHit1.bhvr
		Part1	CustomizeablePowers\EnergyBlast\EnergyShockRing.part
		Lifespan 28
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		1000
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Ename	Ring2
		Type	Start
		At	chest
		bhvr	CustomizeablePowers\EnergyBlast\SpinHit3.bhvr
		Part1	CustomizeablePowers\EnergyBlast\EnergyShockRing.part
		Lifespan 33
	End
End

#############################################################

End