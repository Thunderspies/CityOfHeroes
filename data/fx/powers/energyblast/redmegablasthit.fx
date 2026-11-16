#############################################################
## RedMegaBlastHit.fx
#############################################################

FxInfo

LifeSpan 80

#############################################################

Input
	InpName	Chest
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Ring1
		Type	Start
		At	chest
		bhvr	Behaviors\SpinHit1.bhvr
		Part1	:EnergyShockRingRed.part
		Lifespan 40
	End

	Event
		EName 	hit2
		Type	local
		At	chest
		Part2	:DarkEnergyBallsHitRed.part
		Part3	:DarkEnergyBallsHitRed2.part
		Part4	:BlastHitRingsRed.part
		Part	:WildEnergyBalls.part
		Part	:WildEnergyBallsRed.part
		Part	:WildEnergyBallsWhite.part
		Lifespan 30
	End

	Event
		EName 	Burst
		Type	local
		At	chest
		Part1	:EnergyBallsBurstRed.part
		Part2	:EnergyBallsBurst2Red.part
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
		Part1	:EnergyShockRingRed.part
		bhvr	Behaviors\SpinHit2.bhvr
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Ename	Ring1
		Type	Start
		At	chest
		Part1	:EnergyShockRingRed.part
		bhvr	Behaviors\SpinHit1.bhvr
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
		bhvr	Behaviors\SpinHit3.bhvr
		Part1	:EnergyShockRingRed.part
		Lifespan 33
	End
End

#############################################################

End