#############################################################
## REDEnergyBlastHit.fx
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
		EName 	geom
		Type	local
		At	Chest
		bhvr	Behaviors\SpinHit2.bhvr
		geom	CenterDummy
		Sound vandalblast2 80 80 0.8
	End

	Event
		EName 	hit2
		Type	local
		At	Chest
		Part	:EnergyBallsHitWhite.part
		Part1	:EnergyBallsHitRed2.part
		Part	:EnergyBallsHitBlack.part
		Part2	:DarkEnergyBallsHitRed.part
		Part	:DarkEnergyBallsHitBlack.part
		Part3	:SolidEnergyBallsHitRed.part
		Part	:SolidEnergyBallsHitBlack.part
		Part4	:BlastHitRingsARed2.part
		Part	:WildEnergyBalls.part
		Part	:WildEnergyBallsRed.part
		Part	:WildEnergyBallsWhite.part
		Lifespan 25
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