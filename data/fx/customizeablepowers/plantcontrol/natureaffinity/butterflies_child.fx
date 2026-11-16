#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName ButtflyAnchor
			Flags OneAtATime
			LifeSpan 200
		End

		Event
			At ButtflyAnchor
			Type Local
			EName RotatorAnchor
			BhvrOverride
				EndScale 1 1 1
				PyrRotateJitter 90 0 0
				Spin 0 0.03 0
				SpinJitter 0 0.01 0
				StartJitter 0.2 2 0.2
			End
		End

	End

	Condition
		On Time
		Time 0
		Random 1

		Event
			Anim FX_Butterflies_01
			At RotatorAnchor
			Type Local
			EName ButtflyGeo
			Bhvr behaviors/Scale_Butterflies.bhvr
			BhvrOverride
				EndScale 0.004 0.004 0.004
				PositionOffset 4 1 1 #change this for huge
				PyrRotate 0 135 180
				Spin 0 0 0.05
				SpinJitter 0 0.02 0
				TintGeom 1
				FadeOutLength 60
				#StartColor 255 255 255
				#HueShift 0
				#HueShiftJitter 0
			End
			LifeSpan 80
		End

		Event
			Anim FX_Butterflies_01
			At RotatorAnchor
			Type Local
			EName ButtflyGeo
			Bhvr behaviors/Scale_Butterflies.bhvr
			BhvrOverride
				EndScale 0.0023 0.0023 0.0023
				PositionOffset 4 1 1 #change this for huge
				PyrRotate 0 135 180
				Spin 0 0 0.05
				SpinJitter 0 0.02 0
				TintGeom 1
				FadeOutLength 60
				#StartColor 255 255 255
				#HueShift 0
				#HueShiftJitter 0
			End
			LifeSpan 80
		End

		Event
			Anim FX_Butterflies_01
			At RotatorAnchor
			Type Local
			EName ButtflyGeo
			Bhvr behaviors/Scale_Butterflies.bhvr
			BhvrOverride
				EndScale 0.0015 0.0015 0.0015
				PositionOffset 4 1 1 #change this for huge
				PyrRotate 0 135 180
				Spin 0 0 0.05
				SpinJitter 0 0.02 0
				TintGeom 1
				FadeOutLength 60
				#StartColor 255 255 255
				#HueShift 0
				#HueShiftJitter 0
			End
			LifeSpan 80
		End

	End

End
