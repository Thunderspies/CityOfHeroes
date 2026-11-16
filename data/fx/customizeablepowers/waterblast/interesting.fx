#
FxInfo

	LifeSpan 15000


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName Anchor
			BhvrOverride
				PositionOffset 0 3 0
				Spin 0 0.1 0
			End
		End

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_Grow.bhvr
			BhvrOverride
				TintGeom 1

				Alpha 100
				FadeInLength 10
			End
			Geom FX_WaterSwirl_Add
		End

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_Grow.bhvr
			BhvrOverride
				TintGeom 1
				Alpha 150
				FadeInLength 10
			End
			Geom FX_WaterSwirl
		End

	End

	Condition
		On Time
		Time 7

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_Grow.bhvr
			BhvrOverride
				PyrRotate 0 120 0
				TintGeom 1
				Alpha 100
				FadeInLength 10
			End
			Geom FX_WaterSwirl_Add
		End

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_Grow.bhvr
			BhvrOverride
				PyrRotate 0 120 0
				TintGeom 1
				Alpha 150
				FadeInLength 10
			End
			Geom FX_WaterSwirl
		End

	End

	Condition
		On Time
		Time 16

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_Grow.bhvr
			BhvrOverride
				PyrRotate 0 -120 0
				TintGeom 1
				Alpha 100
				FadeInLength 10
			End
			Geom FX_WaterSwirl_Add
		End

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_Grow.bhvr
			BhvrOverride
				PyrRotate 0 -120 0
				TintGeom 1
				Alpha 150
				FadeInLength 10
			End
			Geom FX_WaterSwirl
		End

	End

End
