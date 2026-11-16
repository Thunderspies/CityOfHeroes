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
		
		Part	:NovaHitFlame.part
		Part	:NovaHitFlame2.part
		#Part	:DarkEnergyBallsHit.part
		#Part	:SolidEnergyBallsHit.part
		#Part	:BlastHitRings.part
		Sound explohit2 200 200 .76
	End

	Event
		EName 	Burst
		Type	start
		At	Chest
		Part	:EnergyStreak.part
		
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Burst
		Type	start
		At	Chest
		Part	:EnergyStreak.part
		
	End

End

Condition
	On 	Time
	Time 	12

	Event
		EName 	Burst
		Type	start
		At	Chest
		Part	:EnergyStreak.part
		
	End

End

Condition
	On 	Time
	Time 	13

	Event
		EName 	hit2
		Type	Destroy
				
	End


End	


End