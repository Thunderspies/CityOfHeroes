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
		CHILDFX	:Child_Slime_Track.fx


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
		CHILDFX	:Child_Slime_Track.fx


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
		CHILDFX	:Child_Slime_Track.fx


	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.5
	Random	1

	Event
		EName	Slime5
		Type	PositOnly
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Idle.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime6
		Type	PositOnly
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Idle.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime5
		Type	PositOnly
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Idle.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime6
		Type	PositOnly
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Idle.part
		Lifespan 	5
		LifespanJitter 	2
	End
End

Condition
	On	Cycle
	Time	3
	Random	1

	Event
		EName	AOERing
		Type	local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End
End

End