#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	200

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0


	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow3.part
		Part2	:ElectricityGlowStar3.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound pp12 50 50 .7
		LifeSpan	70
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		LifeSpan	70
	End
End

Condition
	On Time 
	Time 2

	Repeat	2
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSparkSmall.fx

		Lifespan	120
		LifeSpanJitter	80
		
	End
End

Condition
	On Time 
	Time 2

	Repeat	2

	Event
		HardwareOnly
		EName	Sparkfar2
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSparkSmall.fx

		Lifespan	120
		LifeSpanJitter	80
		
	End

End

Condition
	On Time 
	Time 2

	Repeat	2

	Event
		HardwareOnly
		EName	Sparkfar2
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSparkSmall.fx

		Lifespan	120
		LifeSpanJitter	80
		
	End

End


End
