#
FxInfo

	LifeSpan 60


###########################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_JudgementVorpal_Attack_01 100 100 .8
	End
End



	Condition
		On 		TRIGGERBITS
		TRIGGERBITS 	MALE

		Event
			EName Vorpal_Male
			Anim FX_DimensionShift
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
			Anim FX_DimensionShift
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
			Anim FX_DimensionShift
			At Root
			Flags	OneAtATime
			Type Local
			CHILDFX	:HUGE_Judgement_Vorpal_Attack.fx
			LifeSpan 60
		End
	End
End