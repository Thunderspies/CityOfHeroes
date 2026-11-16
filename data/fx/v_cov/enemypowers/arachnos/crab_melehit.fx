#########################################################
##	template  reference from powers/MedievalWeapons/MaceHit2.fx

FxInfo

Lifespan	90

Input
	Inpname Chest
End

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow01
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		Sound CrabMelee 60.0 60.0 .8
		Lifespan 2
	End


End

Condition
	On Time 
	Time 0
	
	Event
		EName	SoundFX
		Type 	Local
		At	Chest
		Sound knifewhoosh1 60.0 60.0 .3
		Lifespan 2
	End


End

Condition
	On Time 
	Time 5
	
	Event
		EName	SoundFX
		Type 	Local
		At	Chest
		Sound knifewhoosh2 60.0 60.0 .3
		Lifespan 2
	End


End

Condition
	On Time 
	Time 10
	
	Event
		EName	SoundFX
		Type 	Local
		At	Chest
		Sound knifewhoosh1 60.0 60.0 .3
		Lifespan 2
	End


End

Condition
	On Time 
	Time 18
	
	Event
		EName	SoundFX
		Type 	Local
		At	Chest
		Sound knifewhoosh3 60.0 60.0 .3
		Lifespan 2
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



Condition
	On Time 
	Time 4
	
	Event
		EName	Pow02
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		#Sound	MaceHit2 80.0 30.0 .8
		Lifespan 2
	End

End


Condition
	On Time 
	Time 6
	
	Event
		EName	Pow03
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		#Sound	MaceHit 60.0 30.0 .7
		Lifespan 2
	End

End

Condition
	On Time 
	Time 8
	
	Event
		EName	Pow03
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		#Sound	MaceHit2 60.0 30.0 .66
		Lifespan 2
	End

End

Condition
	On Time 
	Time 6
	
	Chance	.75
	Repeat	4
	RepeatJitter	2
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End

End

Condition
	On Time 
	Time 6

	Chance	.75
	Repeat	5
	RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUTbig.fx
		
	End

End


Condition
	On Time 
	Time 16
	
	Event
		EName	Pow03
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		#Sound	MaceHit2 60.0 30.0 .8
		Lifespan 2
	End

End

Condition
	On Time 
	Time 20
	
	Event
		EName	Pow03
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		#Sound	MaceHit 60.0 30.0 .8
		Lifespan 2
	End

End

Condition
	On Time 
	Time 18
	
	Chance	.75
	Repeat	4
	RepeatJitter	2
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End

End

Condition
	On Time 
	Time 18

	Chance	.75
	Repeat	5
	RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End

End
	

End	

	