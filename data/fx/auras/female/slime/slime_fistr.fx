#############################################################
## Slime_Body.fx
#############################################################

FxInfo
Flags DontSuppress

########################################################



## POWER FX #######################################################

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     MALE
	DoMany		1

	Event
		ENAME	Cloud
		Type	local
		At	Root
		WHILE	MALE

		Flags	OneAtATime
		CHILDFX	:Child_Slime_Track_FistR.fx


	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     FEMALE
	DoMany		1

	Event
		ENAME	Cloud
		Type	local
		At	Root
		WHILE	FEMALE

		Flags	OneAtATime
		CHILDFX	:Child_Slime_Track_FistR.fx


	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     HUGE
	DoMany		1

	Event
		ENAME	Cloud
		Type	local
		At	Root
		WHILE	HUGE
		Flags	OneAtATime
		CHILDFX	:Child_Slime_Track_FistR.fx


	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.5
	Random	1

	Event
		EName	Slime4
		Type	PositOnly
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Idle_Hands.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime4
		Type	PositOnly
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Idle_Hands.part
		Lifespan 	5
		LifespanJitter 	2
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.5

	Event
		EName	AOERing
		Type	positonly
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End
End

########################################################

End