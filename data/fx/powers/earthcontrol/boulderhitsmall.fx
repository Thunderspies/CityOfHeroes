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
		#Part3	:WeaponRocks.part
		#Part4	:WeaponRocks06a.part
		#Part5	:WeaponRocks06a.part
		#Bhvr	Behaviors/CameraShake02Subtle.bhvr
		Sound stonehit2 100 100 .85

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
	Repeat	5

	Event
		Type	Local 
		At	chest
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx

		Lifespan	100
		
	End


End

Condition
	On 	Time
	Time 	0
	Repeat	1
	
	Event
		HardwareOnly
		Type	Local 
		At	chest
		
		ChildFX		V_COV\PhysicsEnabled\RockChunkBrownScatter.fx

		Lifespan	90
		LifespanJitter	15
		
	End

End


End
