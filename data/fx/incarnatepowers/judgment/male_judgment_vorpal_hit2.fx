#
FxInfo

	LifeSpan 90


###########################################################

	Condition
		On Time
		Time 10

		Event
			At Root
			Type StartPositOnly
			EName OffsetAnchor
			BhvrOverride
				PyrRotateJitter 0 180 0
			End
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 13

		Event
			At OffsetAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 3 -3
			End
			Part :Burst_Star_Small.part
			Part :Burst_Flash.part
			LifeSpan 20
		End

		Event
			Anim FX_Judgement_Vorpal_Male
			At OffsetAnchor
			Type Local
			EName DummyAnchor
			SetState HIT
			Bhvr :Judgment_Vorpal_Blur.bhvr
			BhvrOverride
				TintGeom	1
				PositionOffset 0 0 -3
			End
			LifeSpan 13
		End

	End

	Condition
		On Time
		Time 25

		Event
			At OffsetAnchor
			Type Local
			BhvrOverride
				TintGeom	1
				PositionOffset 0 4 -2.0
			End
			Part :Burst_Star_Shrink.part
			Part IncarnatePowers\judgment\Burst_Flash_Shrink.part
			LifeSpan 5
		End

	End

End
