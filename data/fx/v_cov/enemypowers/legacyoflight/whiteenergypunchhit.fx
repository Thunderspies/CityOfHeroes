#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	40

###########################################################
##SOUND
#################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	geom
		Type	local
		At	Chest
		Sound legacypunch1 70 70 .86

	End

End

###################################################################################

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
		Part4	:WhiteBlastHitRings.part
		
		LifeSpan	10
	End

	Event
		EName 	Burst
		Type	local
		At	Chest
		#Part1	:WhiteEnergyBallsHit.part
		#Part2	:WhiteDarkEnergyBallsHit.part

		#Part3	:WhiteSolidEnergyBallsHit.part
		Part1	:WhiteEnergyBallsBurst.part
		Part2	:WhiteEnergyBallsBurst2.part

		LifeSpan	4
	
	End

	Event
		EName 	Burst
		Type	local
		At	Chest
		Part1	:WhiteEnergyBallsHit.part
		Part2	:WhiteDarkEnergyBallsHit.part

		Part3	:WhiteSolidEnergyBallsHit.part
		#Part1	:WhiteEnergyBallsBurst.part
		#Part2	:WhiteEnergyBallsBurst2.part
		
		LifeSpan	7

	End

End	


End