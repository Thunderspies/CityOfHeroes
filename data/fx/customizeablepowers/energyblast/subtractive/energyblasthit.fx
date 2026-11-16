#############################################################
## EnergyBlastHit.fx
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
		EName 	geom
		Type	local
		At	Chest
		Bhvr	:SpinHit2.bhvr
		geom	CenterDummy

	End

	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	:EnergyBallsHit.part
		Part2	:DarkEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit_Alpha.part
		Part4	:BlastHitRings.part
		Part4	:BlastHitRings_Alpha.part
		Sound explohit2 200 200 0.76
		Lifespan 25
	End


	Event
		EName 	Burst
		Type	local
		At	Chest
		Part1	:EnergyBallsBurst.part
		Part1	:EnergyBallsBurst_Alpha.part
		Part2	:EnergyBallsBurst2.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		1000
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

End