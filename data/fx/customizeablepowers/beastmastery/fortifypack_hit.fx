###########################################################

FxInfo
Flags DontSuppress
Lifespan 50

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BM_FortifyPack_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At 	Root
			Type 	Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Ring.part
			Part	:FortifyPack_Flash.part
			Lifespan 30
		End

	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	CAT


		Event
			EName	FortifyPack_Lion
			At 	Root
			Type 	Local
			WHILE	CAT
			Flags	OneAtATime
			Bhvr	behaviors/GenericParticleFade.bhvr
			CHILDFX	:CHILD_FortifyPack_Lion.fx
		End
	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	DOG


		Event
			EName	FortifyPack_Wolf
			At 	Root
			Type 	Local
			WHILE	DOG
			Flags	OneAtATime
			Bhvr	behaviors/GenericParticleFade.bhvr
			CHILDFX	:CHILD_FortifyPack_Wolf.fx
		End
	End


###########################################################

End
