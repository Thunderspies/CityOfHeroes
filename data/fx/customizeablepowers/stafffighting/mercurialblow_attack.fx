#
FxInfo

	LifeSpan 15


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SM_MercurialBlow_Attack_01 100 100 .8
	End
End


	Condition
		On		TRIGGERBITS
		TRIGGERBITS 	PUNCH RIGHT

		Event
			EName	StrikeA
			At Hips
			Type Local
			Flags	OneAtATime
			CHILDFX	:CHILD_MercurialBlow_A.fx
			LifeSpan 10
		End
	End



	Condition
		On		TRIGGERBITS
		TRIGGERBITS 	PUNCH LEFT
		DoMany		1
		Event
			EName	StrikeB
			At 	Root
			Type 	Local
			Flags	OneAtATime
			CHILDFX	:CHILD_MercurialBlow_B.fx

			LifeSpan 10
		End
	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS 	PUNCH UP
		DoMany		1
		Event
			EName	StrikeB
			At 	Root
			Type 	Local
			Flags	OneAtATime
			CHILDFX	:CHILD_MercurialBlow_C.fx

			LifeSpan 20
		End
	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS 	PUNCH DOWN
		DoMany		1

		Event
			EName	StrikeA
			At Hips
			Type Local
			Flags	OneAtATime
			CHILDFX	:CHILD_MercurialBlow_D.fx
			LifeSpan 10
		End
	End

End
