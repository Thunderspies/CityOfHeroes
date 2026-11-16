#########################################################
##	
##

FxInfo

LifeSpan	300

#########################################################
###################   Geom Drill   ######################
#########################################################

Condition
	On 	Time
	Time 	0
	


	Event
		EName	Hit
		Type	local
		At	chest
			
		BHVR	:Arachnoid_B_BarbHit.bhvr
	End

	Event
		EName	Barb
		Type	local
		At	Hit
			
		BHVR	:Arachnoid_B_BarbHitShrink.bhvr
		Geom	ArachnoidBarbShrink
		#Lifespan	44
	End

#########################################################
###################   Gas impact   ######################
#########################################################

	Event

		EName	Gas
		Type	Local
		At	chest
				
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_GasHit.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpecksHit.part
		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_GasBubbles.part
		#Sound	Quills3 70 60 .98
		Lifespan 10		
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
				
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_GasHit.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpecksHit.part
		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_GasBubbles.part
		Sound arachnoidpoisonhit 70 70 .84
		Lifespan 10	
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
				
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_GasHit.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpecksHit.part
		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_GasBubbles.part
		Lifespan 10		
	End


	Event

		EName	GasSpurts
		Type	Local
		At	Hit
				
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Sparks.part
		Lifespan	10
	End

	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head
				
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_GasSpecksHead.part
		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_GasBubblesHead.part
		lifespan 12
	End
	
End

#########################################################
####################   Gas Drill   ######################
#########################################################

Condition
	On 	Time
	Time 	0
	


	Event
		EName	DrillGas
		Type	local
		At	Hit
			
		BHVR	:Arachnoid_B_BarbDrill.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpecksDrill.part
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	0
	


	Event
		EName	DrillGas
		Type	local
		At	Hit
			
		BHVR	:Arachnoid_B_BarbDrill01.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpecksDrill01.part
		Lifespan 45
	End
End


#########################################################
##################   Poison Drops   #####################
#########################################################


Condition
	On 	Time
	Time 	0
	Repeat	2
	
	Event
		EName	PoisonDrops
		Type	Local
		At	chest		
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan	75
	End
End


Condition
	On 	Time
	Time 	0
	Repeat	3
	
	Event
		EName	PoisonDrops
		Type	Local
		At	chest		
		ChildFX	V_COV\PhysicsEnabled\PoisonDrip.fx
		Lifespan	75
	End
End

End				