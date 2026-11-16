#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	root
End

LifeSpan	120

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	root
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound lightningbolthit 100 100 .7
	End

	Event
		EName	continuingArchs
		Type	Local
		At	root
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
	End
End

	Condition
	On 	Time
	Time 	60	#35
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
		At	root
		BhvrOverride
			PositionOffset		0 .5 0
		End
		ChildFX	V_COV\PhysicsEnabled\LightningHit.fx

		Lifespan	90
		
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	root
		BhvrOverride
			PositionOffset		0 1 0
		End
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
		At	root
		BhvrOverride
			PositionOffset		0 1 0
		End
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90
		
	End

End


End
