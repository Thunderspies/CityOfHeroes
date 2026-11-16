#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan 50
#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part1	:Clawflash.part
		Part3	:LightClawEmbers.part
		Part4	:LightClawEmbersBroad.part
		Part5	:ClawsStar.part
		Sound clawswipe5 70.0 70.0 .88
	End

End
Condition
	On Time 
	Time 0
	
	Repeat	1
#	RepeatJitter	1
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplodeUP.fx
		
	End

End
Condition
	On Time 
	Time 0
	
	Repeat	1
#	RepeatJitter	1
	
	Event
		EName	Sparkdown
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkUP.fx
		
	End
		
	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
		
	End

	
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplodeUP.fx
		
	End
End

Condition
	On Time 
	Time 0

	Chance	.5
	Repeat	1
#	RepeatJitter	1
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUTbig.fx
		
	End

End

Condition
	On Time 
	Time 2
	
	Chance	.75
	Repeat	1
#	RepeatJitter	1
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
		
	End


End

	

###################  2   #############################

	

Condition
	On Time 
	Time 13
	
	Event
		EName	Pow2
		Type 	Local
		At	Chest
		Part1	:Clawflash.part
		Part3	:LightClawEmbers.part
		Part4	:LightClawEmbersBroad.part
		Part5	:ClawsStar.part
	End

End


Condition
	On Time 
	Time 5
	
	Event
		EName	Pow
		Type 	Destroy
			
	End

End

Condition
	On Time 
	Time 15
	
	Event
		EName	Pow2
		Type 	Destroy
			
	End

End
	

Condition
	On Time 
	Time 13
	
	Repeat	1
	RepeatJitter	1
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkUP.fx
		
	End

		
	Event
		HardwareOnly
		EName	Sparkfast
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplode.fx
		
	End
	
	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
		
	End
End

Condition
	On Time 
	Time 13

	Chance	.5
	Repeat	1
	RepeatJitter	1
	
	Event
		EName	Sparkfast
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplode.fx
		
	End

End

Condition
	On Time 
	Time 14

	Chance	.75
	Repeat	1
	RepeatJitter	1
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUTbig.fx
		
	End

End

Condition
	On Time 
	Time 15
	
	Chance	.75
	Repeat	1
	RepeatJitter	1
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
		
	End


End

	

End	

		

	