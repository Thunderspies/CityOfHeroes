#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	150

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
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Part3	:ElectricityArch4aRed.part
		Part4	:ElectricityArchSmallaRed.part
		Sound lightningbolthit 100 100 .7
		LifeSpan	30
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuingRed.part
		Part2	:ElectricityArchContinuing2Red.part
	End
End


Condition
	On Time 
	Time 5

	Repeat	6
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedLightningHit.fx

		Lifespan	90
		
	End

End

Condition
	On Time 
	Time 2

	Repeat	11
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

		Lifespan	90
		
	End

End


End
