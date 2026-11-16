#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200

#########################################################

Condition
	On 	Time
	Time 	26

	Event
		EName	Emitter
		Type	Local
		At	Head
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitEmit.bhvr
#		Geom	Testing_Target
		Sound arachnoidspit 80 80 .88
	End


	Event
		EName	Prime
		Type	start
		At	Emitter
		
		Bhvr	:Arachnoid_B_BarbTravel.bhvr
		Geom	ArachnoidBarb
		Magnet	T_chest
		LookAt	T_Chest
	End

	Event
		EName	Prime1
		Type	start
		At	Emitter
		
		Bhvr	:Arachnoid_B_BarbTravel.bhvr
#		Part1	weaponFx/QuillTrail.part
#		Part2	weaponFx/QuillTrailSpecks.part
#		Part3	weaponFx/QuillBubblesTrail.part
#		Part5	weaponFx/QuillBallzTrail.part
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName	Prime2
		Type	start
		At	Emitter
		
		Bhvr	:Arachnoid_B_BarbTravel.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_BarbTrail.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_BarbTrailSpecs.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_BarbTrailSpecs01.part
		Magnet	T_Chest
		LookAt	T_Chest
		
	End

	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
		

	End

End

End				