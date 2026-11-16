#                                                          
FxInfo

	LifeSpan 45


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName dummy
			Part :IntelligenceRingTarget.part
			Part :Endurance02.part
		End

	End

	Condition
		On Cycle
		Time 14
		Chance 1

		Event
			At Chest
			Type Local
			EName dummy
			Part :IntelligenceRingTarget.part
			#Part :Endurance02.part
			LifeSpan 20
		End

	End

	Condition
		On Cycle
		Time 28
		Chance 1

		Event
			At Chest
			Type Local
			EName dummy
			Part :IntelligenceRingTarget.part
			#Part :Endurance02.part
			LifeSpan 20
		End

	End

End
