#                                                          
FxInfo

	LifeSpan 460


###########################################################

	Condition

		Event
			At Chest
			Type StartPositOnly
			EName RayOffsetHigh
			BhvrOverride
				PositionOffset 0 180 0
			End
		End

		Event
			At Chest
			Type StartPositOnly
			EName BottomOffsetHigh
			BhvrOverride
				PositionOffset 0 25 0
			End
		End

		Event
			At Chest
			Type StartPositOnly
			EName BottomOffsetMid
			BhvrOverride
				PositionOffset 0 50 0
			End
		End

		Event
			At Root
			Type StartPositOnly
			EName BottomOffsetLow
			BhvrOverride
				PositionOffset 0 -10 0
			End
		End

		Event
			At RayOffsetHigh
			Type StartPositOnly
			EName Ray
			POther BottomOffsetHigh
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_LightningBroken.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_Core2.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_Glow.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_Core.part
		End

		Event
			At RayOffsetHigh
			Type StartPositOnly
			EName Ray_Lightning
			POther BottomOffsetLow
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_Lightning.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_Lightning_Inverted.part
		End

		Event
			At BottomOffsetMid
			Type StartPositOnly
			EName RayLow
			POther Chest
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_Lightning_Inverted_Small.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_LightningBroken_Small.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_Core2_Small.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_Core_Small.part
		End

		Event
			At Chest
			Type Local
			EName ChestExplode
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_LightningBroken_Chest.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay_ChestExplode.part
		End

	End

	Condition
		On Cycle
		Time 5

		Event
			Type SetLight
			EName BodyLight
			Bhvr Scripted\SignatureStories\OneWillDie\Statesman\BodyLight.bhvr
			LifeSpan 5
		End

	End

End
