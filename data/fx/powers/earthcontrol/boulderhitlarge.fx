#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Target
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	Local 
		At	Chest
		Part1	:StalagtiteDustHit.part
		Part2	:StalagtiteDustHit2.part
		#Part3	:hitRocks06.part
		#Part4	:BoulderBreakgrey.part
		#Part5	:hitRocks06.part
		#Bhvr	Behaviors/CameraShake02Subtle.bhvr
		Sound boulderhit 90 90 .99

	End


End

Condition
	On 	Time
	Time 	0
	Repeat	19

	Event
		Type	Local 
		At	chest
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGold.fx

		Lifespan	100
		
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	15

	Event
		Type	Local 
		At	chest
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx

		Lifespan	100
		
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		Type	Local 
		At	chest
		
		BhvrOverride
			
			FadeOutLength 		160

		End

		ChildFX		V_COV\PhysicsEnabled\RockChunkBrownScatter.fx

#		Lifespan	90
#		LifespanJitter	15
		
	End

End


Condition
	On 	Time
	Time 	0
	Repeat	15

	Event
		Type	Local 
		At	chest
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx

		Lifespan	100
		
	End

End

End
