#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	80

Input  
	InpName	Chest
End


###########################################################
##Home
###########
Condition
	On 	Time
	Time 	0
	
	Event	
		Ename   Ice
		Type	local
		At	chest
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:IceChunks.part
		Part2	Powers/ColdControl/IceChunksLarge.part
#		Part	Powers/ColdControl/iceScatter.part		
		LifeSpan	43
		
	End

	Event	
		Ename   IceShards
		Type	local
		At	chest
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	Powers/ColdControl/IceBoltMistA.part
		Part2	Powers/ColdControl/ChillTouchSnowA.part
		Part4	Powers/ColdControl/ChillTouchSnow.part
		Sound Ice7 100.0 100.0 1.0
		LifeSpan	33
	End

###################################################

	Event
		EName 	hit2
		Type	local
		At	Chest

		Part1	Powers/energyBlast/EnergyBallsHit.part
		Part	Powers/energyBlast/EnergyBallsHitWhite.part
		Part3	Powers/energyBlast/SolidEnergyBallsHit2.part
		Part4	Powers/energyBlast/BlastHitRingsa.part
		Sound wolfpackenergyblasthit2 70 70 .7
	End


	Event
		EName 	Burst
		Type	local
		At	Chest
		
		Part2	Powers/energyBlast/EnergyBallsBurst2.part

	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	hit2
		Type	Destroy
				
	End


End	


End