#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	80

Input  
	InpName	Root
End


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
		At	Root
		
		geom	fireAura

	End
	
	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	:EnergyBallsHit.part
		Part2	:DarkEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit.part
		#Part4	:BlastHitRings.part
		Sound explohit 80 80 .75
	End


	Event
		EName 	Burst
		Type	local
		At	Chest
		Part1	:EnergyBallsBurst.part
		Part2	:EnergyBallsBurst2.part
	End

	Event
		EName 	Ring
		Type	start
		At	geom
		Altpiv	2
		#Part1	:ExplosionHitRing.part
		#Part2	:ExplosionHitRing2.part
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