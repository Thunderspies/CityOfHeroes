#
FxInfo

	Flags DontSuppress NotCompatibleWithAnimalHead


###########################################################

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

###########################################################

End
