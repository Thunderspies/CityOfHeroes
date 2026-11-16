#############################################################################
##Head dizzy
########################################################
FxInfo

Input  
	Inpname	head
End

Condition
	On 	cycle
	Time 	17
	Chance	.04	#.4

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging	
		bhvr	Behaviors/AnimalEyesScale.bhvr
		LifeSpan	25
	End

	Event
		Type	Local
		At	core
		altpiv	1
		Part1	:AnimalEyesWhite.part
		Part2	:AnimalEyesGlowWhite.part
		LifeSpan	25
	End

	Event
		Type	Local
		At	core
		altpiv	3
		Part1	:AnimalEyesWhite.part
		Part2	:AnimalEyesGlowWhite.part
		LifeSpan	25
	End
End


End
