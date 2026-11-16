#
FxInfo

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName Anchor
			BhvrOverride
				Spin 0 0.1 0
			End
		End

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_GrowBurst.bhvr
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
			Bhvr :WaveSwirl_GrowBurst.bhvr
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
			Bhvr :WaveSwirl_GrowBurst.bhvr
			BhvrOverride
				PyrRotate 0 120 0
				Alpha 100
				FadeInLength 10
			End
			Geom FX_WaterSwirl_Add
		End

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_GrowBurst.bhvr
			BhvrOverride
				PyrRotate 0 120 0
				Alpha 150
				FadeInLength 10
			End
			Geom FX_WaterSwirl
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Anchor
			Type Local
			Bhvr :WaveSwirl_GrowBurst.bhvr
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
			Bhvr :WaveSwirl_GrowBurst.bhvr
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
