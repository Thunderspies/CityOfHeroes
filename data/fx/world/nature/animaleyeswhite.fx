#############################################################################
##Head dizzy
########################################################
FxInfo

Input  
	Inpname	head
End

Condition
	On 	cycle
	Time 	7
	Chance	.03	

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging	
		bhvr	Behaviors/AnimalEyesScaleSmall.bhvr
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
