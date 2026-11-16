#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Start
			POther Head
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 4

		Event
			At Hips
			Type Start
			POther Chest
			Part :Overload_BodyGlow.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			LifeSpan 10
		End

		Event
			At Head
			Type Start
			Part :Overload_BodyGlow.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 6

		Event
			At LArmR
			Type Start
			POther UArmR
			Part :Overload_BodyGlow.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 8

		Event
			At LArmL
			Type Start
			POther UArmL
			Part :Overload_BodyGlow.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			LifeSpan 10
		End

		Event
			At WepR
			Type Start
			POther LArmR
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow_Small.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 12

		Event
			At WepL
			Type Start
			POther LArmL
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow_Small.part
			LifeSpan 10
		End

		Event
			At LLegR
			Type Start
			POther ULegR
			Part :Overload_BodyGlow.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 14

		Event
			At LLegR
			Type Start
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow_Small.part
			LifeSpan 10
		End

		Event
			At LLegL
			Type Start
			POther ULegL
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow_Small.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 18

		Event
			At LLegL
			Type Start
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow_Small.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 19

		Event
			At FootR
			Type Start
			POther LLegR
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow_Small.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 20

		Event
			At FootL
			Type Start
			POther LLegL
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\Overload_BodyGlow_Small.part
			LifeSpan 10
		End

	End

End
