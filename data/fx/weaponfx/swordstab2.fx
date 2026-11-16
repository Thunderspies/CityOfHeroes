#########################################################
##	template

FxInfo
LifeSpan	105

Input
	Inpname chest
End


Condition
	
	Event	
	Type 	Local
	At	chest
	Sound SwordStab2 100.0 100.0 .8

	End

End

Condition
	On Time 
	Time 0
	
	Repeat	3
	RepeatJitter	2
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End
		
	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End
End

Condition
	On Time 
	Time 2
	
	Chance	.5
	Repeat	2
	RepeatJitter	1
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End
	
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End

End

End

	

	