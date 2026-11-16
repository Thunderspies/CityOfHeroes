#########################################################
##	EBlast
########################################################
FxInfo

LifeSpan	100

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	origin
#		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchz.part
	
		
		LifeSpan	10
	End

	Event
		EName	continuingArchs
		Type	Local
		At	origin
#		Part1	:ElectricityArchContinuing.part
#		Part2	:ElectricityArchContinuing2.part
		LifeSpan	5
	End
End

Condition
	On Time 
	Time 5

	Repeat	2
	
	Event
		EName	Sparkfar
		Type 	Start
		At	origin
		ChildFX	V_COV\PhysicsEnabled\LightningHitLight.fx

		Lifespan	90
		
	End

End

#Condition
#	On Time 
#	Time 2
#
#	Repeat	4
#	
#	Event
#		EName	Sparkfar
#		Type 	Start
#		At	origin
#		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
#
#		Lifespan	90
#		
#	End
#
#End


End
