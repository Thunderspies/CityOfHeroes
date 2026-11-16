#
FxInfo

	LifeSpan 60


###########################################################

	Condition
		On 		TRIGGERBITS
		TRIGGERBITS 	MALE

		Event
			EName Vorpal_Male
			At Root
			Flags	OneAtATime
			Type Local
			CHILDFX	:MALE_Judgement_Vorpal_Attack.fx
			LifeSpan 60
		End
	End

	Condition
		On 		TRIGGERBITS
		TRIGGERBITS 	FEMALE

		Event
			EName Vorpal_Huge
			At Root
			Flags	OneAtATime
			Type Local
			CHILDFX	:FEMALE_Judgement_Vorpal_Attack.fx
			LifeSpan 60
		End
	End

	Condition
		On 		TRIGGERBITS
		TRIGGERBITS 	HUGE

		Event
			EName Vorpal_Huge
			At Root
			Flags	OneAtATime
			Type Local
			CHILDFX	:HUGE_Judgement_Vorpal_Attack.fx
			LifeSpan 60
		End
	End
End