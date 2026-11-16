#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		CHILDFX	:CHILD_Electricity_STATIONARY.fx
	End
End

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

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     MALE
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	MALE
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_NOAIR.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS    	FEMALE
	DoMany		1

	Event
		ENAME	TrailGroundF
		Type	Local
		At	Root
		WHILE 	FEMALE
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_NOAIR.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS    	HUGE
	DoMany		1

	Event
		ENAME	TrailGround
		Type	Local
		At	Root
		WHILE 	HUGE
		Flags	OneAtATime
		ChildFX	:CHILD_Electricity_Trail_NOAIR.fx
	End
End
#########################################################

End