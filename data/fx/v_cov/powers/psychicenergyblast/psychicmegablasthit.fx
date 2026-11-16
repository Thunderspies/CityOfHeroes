#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	180

Input  
	InpName	Chest
End


Input  
	InpName	Target
End


###########################################################

Condition
	On 	Time
	Time 	0

	

	Event
		Ename	Ring1
		Type	Start
		At	chest
		Part1	:EnergyShockRing.part
		bhvr	Behaviors/SpinHit1.bhvr
	End

	Event
		EName 	hit2
		Type	local
		At	chest
		Part1	:EnergyBallsHit.part
		Part2	:DarkEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit.part
		Part4	:BlastHitRings.part
	End


	Event
		EName 	Burst
		Type	local
		At	chest
		Part1	:EnergyBallsBurst.part
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

		Lifespan	10

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

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	7
	
	Event
		Ename	Ring1
		Type	Start
		At	chest
		Part1	:EnergyShockRing.part
		bhvr	Behaviors/SpinHit2.bhvr
	End

End

Condition
	On 	Time
	Time 	12
	
	Event
		Ename	Ring1
		Type	Start
		At	chest
		Part1	:EnergyShockRing.part
		bhvr	Behaviors/SpinHit1.bhvr
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

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	17
	
	Event
		Ename	Ring2
		Type	Start
		At	chest
		Part1	:EnergyShockRing.part
		bhvr	Behaviors/SpinHit3.bhvr
	End

End

Condition
	On 	Time
	Time 	30
	Event
		EName 	Ring
		Type	Destroy
				
	End

	Event
		EName 	Hit2
		Type	Destroy
				
	End

End

Condition
	On 	Time
	Time 	40
	Event
		EName 	Ring1
		Type	Destroy
				
	End

	
End

Condition
	On 	Time
	Time 	50
	Event
		EName 	Ring2
		Type	Destroy
				
	End

	
End

Condition
	On 	Time
	Time 	80

	Event
		EName 	All
		Type	Destroy
				
	End


End	


End