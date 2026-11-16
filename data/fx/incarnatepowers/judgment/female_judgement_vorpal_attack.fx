#
FxInfo

	LifeSpan 54


###########################################################

	Condition
		On Time
		Time 0

		Event
			Anim FX_DimensionShift
			At Root
			Type Local
			EName DummyAnchor
			LifeSpan 105
		End

	End

	Condition
		Time 10

		Event
			At Hips
			Type Start
			Part IncarnatePowers\judgment\Burst_Hurricane_Bright.part
			Part IncarnatePowers\judgment\Burst_Sparks.part
			Part IncarnatePowers\judgment\Burst_Star_Initial.part
			Part IncarnatePowers\judgment\Burst_Hurricane.part
			Part IncarnatePowers\judgment\Burst_Ring.part
			LifeSpan 5
		End

	End

	Condition
		Time 12

		Event
			At Hips
			Type Start
			Part IncarnatePowers\judgment\Burst_Streak_Initial.part
			Part IncarnatePowers\judgment\Burst_Sparks.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 4

		Event
			Anim FX_Judgement_Vorpal_Female
			AnimPiv UArmL
			At DummyAnchor
			Type Local
			Bhvr :Judgment_Vorpal_Blur.bhvr
			BhvrOverride
				TintGeom	1
			End
			LifeSpan 5
		End

		Event
			Anim FX_Judgement_Vorpal_Female
			AnimPiv UArmR
			At DummyAnchor
			Type Local
			Bhvr :Judgment_Vorpal_Blur.bhvr
			BhvrOverride
				TintGeom	1
			End
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 8

		Event
			Anim FX_Judgement_Vorpal_Female
			AnimPiv LArmL
			At DummyAnchor
			Type Local
			Bhvr :Judgment_Vorpal_Blur.bhvr
			BhvrOverride
				Alpha		150
				TintGeom	1
			End
			LifeSpan 5
		End

		Event
			Anim FX_Judgement_Vorpal_Female
			AnimPiv LArmR
			At DummyAnchor
			Type Local
			Bhvr :Judgment_Vorpal_Blur.bhvr
			BhvrOverride
				Alpha		150
				TintGeom	1
			End
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 12

		Event
			Anim FX_Judgement_Vorpal_Female
			AnimPiv HandL
			At DummyAnchor
			Type Local
			Bhvr :Judgment_Vorpal_Blur.bhvr
			BhvrOverride
				Alpha		100
				TintGeom	1
			End
			LifeSpan 5
		End

		Event
			Anim FX_Judgement_Vorpal_Female
			AnimPiv HandR
			At DummyAnchor
			Type Local
			Bhvr :Judgment_Vorpal_Blur.bhvr
			BhvrOverride
				Alpha		100
				TintGeom	1
			End
			LifeSpan 5
		End

		Event
			At Root
			Type Local
			Flags AdoptParentEntity
			Bhvr behaviors\fadeOut.bhvr
			BhvrOverride
				FadeOutLength 12
			End
			LifeSpan 1
		End

	End

	Condition
		On	Time
		Time	50

		Event
			At Chest
			Type Local
			Part :Burst_Star_Small.part
			Part :Burst_Flash.part
			LifeSpan 20
		End
	End

End
