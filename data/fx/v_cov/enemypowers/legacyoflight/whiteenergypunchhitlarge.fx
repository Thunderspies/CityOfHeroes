#########################################################
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
		Sound legacyenergypunchhit 77 77 .76

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
		Geom	SuperStrengthBubble
		bhvr	:ScaleBubble.bhvr

		LifeSpan	2
	End

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