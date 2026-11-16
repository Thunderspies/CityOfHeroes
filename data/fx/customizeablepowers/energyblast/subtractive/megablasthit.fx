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
		Bhvr	:SpinHit1.bhvr
		Part1	:EnergyShockRing.part
		Sound Megablasthit2 80 80 .95
		Lifespan 30
	End

	Event
		EName 	hit2
		Type	local
		At	chest
		Part1	:EnergyBallsHit.part
		Part2	:DarkEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit_Alpha.part

		Part4	:BlastHitRings.part
		Part4	:BlastHitRings_Alpha.part

		Lifespan 30
	End

	Event
		EName 	Burst
		Type	local
		At	chest
		Part1	:EnergyBallsBurst.part
		Part1	:EnergyBallsBurst_Alpha.part
		Part2	:EnergyBallsBurst2.part
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
		bhvr	:SpinHit2.bhvr
		Part1	:EnergyShockRing.part
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Ename	Ring1
		Type	Start
		At	chest
		bhvr	:SpinHit1.bhvr
		Part1	:EnergyShockRing.part
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
		bhvr	:SpinHit3.bhvr
		Part1	:EnergyShockRing.part
		Lifespan 33
	End
End

#############################################################

End