#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At UArmL
			Type PositOnly
			POther Head
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Glow.part
		End

		Event
			At Head
			Type PositOnly
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Glow.part
		End

		Event
			At WepR
			Type PositOnly
			POther LArmR
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Glow.part
		End

		Event
			At WepL
			Type PositOnly
			POther LArmL
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Glow.part
		End

		Event
			At LLegR
			Type PositOnly
			POther ULegR
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Glow.part
		End

		Event
			At LLegL
			Type PositOnly
			POther ULegR
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Glow.part
		End

	End

	Condition
		On Cycle
		Time 10
		Random 1
		Chance 0.5

		Event
			At Chest
			Type Start
			POther Head
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Electricity.part
			LifeSpan 5
		End

		Event
			At UArmR
			Type Start
			POther Head
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Electricity.part
			LifeSpan 2
		End

		Event
			At UArmL
			Type Start
			POther Head
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Electricity.part
			LifeSpan 2
		End

		Event
			At Head
			Type Start
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Electricity.part
			LifeSpan 2
		End

		Event
			At WepR
			Type Start
			POther LArmR
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Electricity.part
			LifeSpan 2
		End

		Event
			At WepL
			Type Start
			POther LArmL
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Electricity.part
			LifeSpan 2
		End

		Event
			At LLegR
			Type Start
			POther ULegR
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Electricity.part
			LifeSpan 2
		End

		Event
			At LLegL
			Type Start
			POther ULegR
			Part Scripted\SignatureStories\OneWillDie\SigArc5\BodyStatic_Electricity.part
			LifeSpan 2
		End

	End

End
