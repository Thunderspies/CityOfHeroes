#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#############################################################
Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	Domany 1

	Event
		Ename	MaleAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	MALE
		ChildFX	:CHILD_Electricity_Impact_NOFLY.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	Domany 1

	Event
		Ename	FemaleAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	FEMALE
		ChildFX	:CHILD_Electricity_Impact_NOFLY.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	Domany 1

	Event
		Ename	HugeAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	HUGE
		ChildFX	:CHILD_Electricity_Impact_NOFLY.fx
	End
End

#############################################################

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     AIR
	DoMany		1

	Event
		ENAME	TrailAIR
		Type	Local
		At	Root
		WHILE 	AIR
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_AIR.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     FLY
	DoMany		1

	Event
		ENAME	TrailFLY
		Type	Local
		At	Root
		WHILE 	FLY
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_AIR.fx
	End
End

#############################################################

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     FORWARD
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	FORWARD
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_NOAIR_MOVEMENT.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     BACKWARD
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	BACKWARD
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_NOAIR_MOVEMENT.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     STEPLEFT
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	STEPLEFT
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_NOAIR_MOVEMENT.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     STEPRIGHT
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	STEPRIGHT
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_NOAIR_MOVEMENT.fx
	End
End

#########################################################

End