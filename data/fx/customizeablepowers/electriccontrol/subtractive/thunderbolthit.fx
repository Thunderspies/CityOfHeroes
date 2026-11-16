#########################################################
##	EBlast
########################################################
FxInfo

Input
	Inpname	Hips
End

LifeSpan	170

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	5

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound Elecballhit 80 80 .8
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
	End
End

	Condition
	On 	Time
	Time 	45
	Event
		EName 	HandGlow1
		Type	Destroy

	End

End

Condition
	On Time
	Time 5

	Repeat	8

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightningHit.fx

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
	On Time
	Time 2

	Repeat	5

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
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


End
