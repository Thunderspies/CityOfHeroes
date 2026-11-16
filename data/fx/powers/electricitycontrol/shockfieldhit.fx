#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	 Target
End

Input  
	Inpname	Root
End

LifeSpan	100

########################################################


Condition
	On 	Time
	Time 	5

	Event
		
		Type	Local
		At	Target
		#Part2	:ShockFieldGlow.part
		Part3	:ShockFieldArchs2.part
		Part4	:ShockFieldArchs3.part
		
	End


End

Condition
	On Time 
	Time 5

	Repeat	2
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Target
		ChildFX	V_COV\PhysicsEnabled\LightningHit.fx

		Lifespan	90
		
	End

#	Event
#		HardwareOnly
#		EName	Sparkfar
#		Type 	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
#
#		Lifespan	90
#		
#	End

End

Condition
	On Time 
	Time 2

	Repeat	6
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Target
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90
		
	End
	
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90
		
	End
End


Condition
	On Cycle
	Time 12

	Chance	.5
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Target
		ChildFX	V_COV\PhysicsEnabled\LightningHit.fx

		Lifespan	15
		
	End

End

End
