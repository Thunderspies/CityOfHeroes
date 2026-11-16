#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	80

Input  
	InpName	Chest
End


###########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	geom
		Type	local
		At	Chest
		bhvr	Behaviors/SpinHit2.bhvr
		geom	CenterDummy

	End
	
	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	:EnergyBallsHit.part
		Part2	:DarkEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit.part
		Part4	:BlastHitRings.part
		Sound explohit2 200 200 .76
	End


	Event
		EName 	Burst
		Type	local
		At	Chest
		Part1	:EnergyBallsBurst.part
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

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	hit2
		Type	Destroy
				
	End


End	


End