#########################################################
##	template

FxInfo


Lifespan	90

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part1	:Clawflash.part
		Part3	:ClawEmbers.part
		Part4	:ClawEmbersBroad.part
		Part5	:ClawsStar.part
		Sound Demon_ClawHit_01 100.0 100.0 .77
		Sound Demon_ClawHit_02 100.0 100.0 .77
		Sound Demon_ClawHit_03 100.0 100.0 .77
	End

End

Condition
	On Time 
	Time 2
	
	Event
		EName	Pow
		Type 	Destroy
			
	End

End

Condition
	On Time 
	Time 0
	
	Repeat	5
	RepeatJitter	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End
		
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End
End

Condition
	On Time 
	Time 0

	Chance	.5
	Repeat	3
	RepeatJitter	1
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUTbig.fx
		
	End

End

Condition
	On Time 
	Time 2
	
	Chance	.75
	Repeat	4
	RepeatJitter	2
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End


End

	

End	

	

	