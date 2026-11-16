#                                                          
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 0
		Random 1

		Event
			At Hips
			Type Local
			EName Swirls_Chest
			POther Head
			Part :CallRavens_Dust_Swirling.part
		End

	End

	Condition
		On Time
		Time 0
		Random 1

		Event
			At Chest
			Type Local
			EName Crows_Chest
			PMagnet Head
			Part :CallRavens_Swirling.part
			Part :CallRavens_Swirling2.part
		End

		Event
			At Hips
			Type Local
			EName Crows_Hips
			PMagnet Chest
			Part :CallRavens_Swirling.part
			Part :CallRavens_Swirling2.part
		End

	End

End
