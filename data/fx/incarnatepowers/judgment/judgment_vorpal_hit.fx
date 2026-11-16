#
FxInfo

	LifeSpan 90
	Flags	UseTargetSeq


###########################################################

Condition
	On 	Time
	Event
		Type	Local
		At	Root
		Sound 	N_Hit_01 100 100 .8
		Sound 	N_Hit_02 100 100 .8
		Sound 	N_Hit_03 100 100 .8
		Sound 	N_Hit_04 100 100 .8
		Sound 	N_Hit_05 100 100 .8
	End
End


	Condition
		On 		TRIGGERBITS
		TRIGGERBITS 	MALE
		Random	1

		Event
			EName Dummy1_Male
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:MALE_Judgment_Vorpal_Hit.fx
			LifeSpan 20
		End

		Event
			EName Dummy2_Male
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:MALE_Judgment_Vorpal_Hit2.fx
			LifeSpan 30
		End

		Event
			EName Dummy3_Male
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:MALE_Judgment_Vorpal_Hit3.fx
			LifeSpan 30
		End

	End

	Condition
		On 		TRIGGERBITS
		TRIGGERBITS 	FEMALE
		Random	1

		Event
			EName Dummy1_Huge
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:FEMALE_Judgment_Vorpal_Hit.fx
			LifeSpan 20
		End

		Event
			EName Dummy2_Huge
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:FEMALE_Judgment_Vorpal_Hit2.fx
			LifeSpan 30
		End

		Event
			EName Dummy3_Huge
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:FEMALE_Judgment_Vorpal_Hit3.fx
			LifeSpan 30
		End

	End

	Condition
		On 		TRIGGERBITS
		TRIGGERBITS 	HUGE
		Random	1

		Event
			EName Dummy1_Huge
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:Huge_Judgment_Vorpal_Hit.fx
			LifeSpan 20
		End

		Event
			EName Dummy2_Huge
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:Huge_Judgment_Vorpal_Hit2.fx
			LifeSpan 30
		End

		Event
			EName Dummy3_Huge
			At Root
			Type StartPositOnly
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			Flags	OneAtATime
			CHILDFX	:Huge_Judgment_Vorpal_Hit3.fx
			LifeSpan 30
		End

	End
End
