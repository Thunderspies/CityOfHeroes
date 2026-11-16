###########################################################

FxInfo

###########################################################

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
		CHILDFX	:PackBuff_ShadowHound_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	DIREWOLF CAT
	DoMany		1

	Event
		EName	FortifyPack_DireWolf_Continuing
		At 	Hips
		Type 	Local
		WHILE	DIREWOLF CAT
		Flags	OneAtATime
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:PackBuff_Wodan_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	BIGDOG2
	DoMany		1

	Event
		EName	FortifyPack_DireWolf_Continuing
		At 	Hips
		Type 	Local
		WHILE	BIGDOG2
		Flags	OneAtATime
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:PackBuff_Wodan_CHILD.fx
	End
End

#Condition
#	On		TRIGGERBITS
#	TRIGGERBITS	BIGDOG3
#	DoMany		1
#
#	Event
#		EName	FortifyPack_DireWolf_Continuing
#		At 	Hips
#		Type 	Local
#		WHILE	BIGDOG3
#		Flags	OneAtATime
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		CHILDFX	:PackBuff_ShadowHunterDog_CHILD.fx
#	End
#End

###########################################################

End
