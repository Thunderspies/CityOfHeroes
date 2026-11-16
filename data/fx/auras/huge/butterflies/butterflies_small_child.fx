#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
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
				Spin 0 0.05 0
				SpinJitter 0 0.03 0
				StartJitter 0.1 0.1 0.1
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
			Bhvr :Scale_Butterflies_Small.bhvr
			BhvrOverride
				EndScale 0.002 0.002 0.002
				PositionOffset .3 .3 .3 #change this for huge
				PyrRotate 0 135 180
				Spin 0 0 0.05
				SpinJitter 0 0.02 0
				TintGeom 1
				FadeOutLength 60
				#StartColor 255 255 255
				#HueShift 0
				#HueShiftJitter 0
			End
			LifeSpan 50
		End

		Event
			Anim FX_Butterflies_01
			At RotatorAnchor
			Type Local
			EName ButtflyGeo
			Bhvr :Scale_Butterflies_Small.bhvr
			BhvrOverride
				EndScale 0.0015 0.0015 0.0015
				PositionOffset .3 .3 .3 #change this for huge
				PyrRotate 0 135 180
				Spin 0 0 0.05
				SpinJitter 0 0.02 0
				TintGeom 1
				FadeOutLength 60
				#StartColor 255 255 255
				#HueShift 0
				#HueShiftJitter 0
			End
			LifeSpan 50
		End

		Event
			Anim FX_Butterflies_01
			At RotatorAnchor
			Type Local
			EName ButtflyGeo
			Bhvr :Scale_Butterflies_Small.bhvr
			BhvrOverride
				EndScale 0.001 0.001 0.001
				PositionOffset .3 .3 .3 #change this for huge
				PyrRotate 0 135 180
				Spin 0 0 0.05
				SpinJitter 0 0.02 0
				TintGeom 1
				FadeOutLength 60
				#StartColor 255 255 255
				#HueShift 0
				#HueShiftJitter 0
			End
			LifeSpan 50
		End

	End

End
