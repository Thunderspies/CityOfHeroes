###########################################################

FxInfo

###########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	CAT
	DoMany		1

	Event
		EName	FortifyPack_Lion_Continuing
		At 	Root
		Type 	Local
		WHILE	CAT
		Flags	OneAtATime
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:CHILD_FortifyPack_Lion_Cycle.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	DOG
	DoMany		1

	Event
		EName	FortifyPack_Wolf_Continuing
		At 	Hips
		Type 	Local
		WHILE	DOG
		Flags	OneAtATime
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:CHILD_FortifyPack_Wolf_Cycle.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	DIREWOLF
	DoMany		1

	Event
		EName	FortifyPack_DireWolf_Continuing
		At 	Hips
		Type 	Local
		WHILE	DIREWOLF
		Flags	OneAtATime
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:CHILD_FortifyPack_DireWolf_Cycle.fx
	End
End

###########################################################

End
