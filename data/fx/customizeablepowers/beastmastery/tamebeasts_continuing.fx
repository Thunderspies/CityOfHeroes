#
FxInfo

	Flags DontSuppress NotCompatibleWithAnimalHead


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BM_TameBeasts_Hit_01 100 100 .8
	End
End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	Dog
		DoMany	1

		Event
			At 	Head
			Type 	Local
			While 	Dog
			Flags	OneAtATime
			EName 	EyeAnchor
			Bhvr 	behaviors/GenericParticleFade.bhvr
			CHILDFX :CHILD_TameBeasts_Wolf.fx
		End
	End


	Condition
		On		TRIGGERBITS
		TRIGGERBITS	Cat
		DoMany	1

		Event
			At 	Head
			Type 	Local
			While 	Cat
			Flags	OneAtATime
			EName 	EyeAnchor
			Bhvr 	behaviors/GenericParticleFade.bhvr
			CHILDFX :CHILD_TameBeasts_Lion.fx
		End
	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	DIREWOLF
		DoMany	1

		Event
			At 	Head
			Type 	Local
			While 	DIREWOLF
			Flags	OneAtATime
			EName 	EyeAnchor
			Bhvr 	behaviors/GenericParticleFade.bhvr
			CHILDFX :CHILD_TameBeasts_DireWolf.fx
		End
	End

###########################################################

End
