#                                                          
FxInfo

	Input
		InpName Chest
	End

	LifeSpan 10


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Start
			EName Mallet
			Part :MartialArtsHit.part
			Part :MartialArtsHit1.part
			Part :MartialArtsHit2.part
			Part :MartialArtsHitRaysSmall.part
			Part :MAHitGlow01.part
			Sound makick 60 60 1.0
		End

	End

End
