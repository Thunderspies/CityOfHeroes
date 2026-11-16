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
		#Part3	:WeaponRocks06a.part
		#Part4	:BoulderBreaka.part
		#Part5	:WeaponRocks06a.part
		#Bhvr	Behaviors/CameraShake02Subtle.bhvr
		Sound smallrockhit1 70 70 .88
		Sound smallrockhit2 70 70 .88

	End


End

Condition
	On 	Time
	Time 	0
	Repeat	2

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
	Repeat	11

	Event
		Type	Local 
		At	chest
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx

		Lifespan	100
		
	End

End


End
