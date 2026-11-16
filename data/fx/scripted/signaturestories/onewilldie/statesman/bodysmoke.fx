#                                                          
FxInfo

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 0

		Event
			At UArmL
			Type PositOnly
			POther Head
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodySmoke_Smoke.part
		End

		Event
			At Head
			Type PositOnly
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodySmoke_Smoke.part
		End

		Event
			At WepR
			Type PositOnly
			POther LArmR
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodySmoke_Smoke.part
		End

		Event
			At WepL
			Type PositOnly
			POther LArmL
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodySmoke_Smoke.part
		End

		Event
			At LLegR
			Type PositOnly
			POther ULegR
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodySmoke_Smoke.part
		End

		Event
			At LLegL
			Type PositOnly
			POther ULegR
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodySmoke_Smoke.part
		End

	End

End
