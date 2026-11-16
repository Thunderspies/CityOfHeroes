#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	20

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
		Part1	:EnergyBallsHitRed.part
		Part2	:DarkEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit.part
		Part4	:BlastHitRings.part
		Sound explohit2 200 200 .76
	End

	Event
		EName 	Burst
		Type	start
		At	Chest
		Part	:EnergyStreakBurst.part
		
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Burst
		Type	start
		At	Chest
		Part	:EnergyStreakBurst.part
		
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