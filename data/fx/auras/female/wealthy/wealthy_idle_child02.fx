#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Start
			EName MoneyAnchor
			Flags OneAtATime
			LifeSpan 200
		End

	End

	Condition
		On Time
		Time 0

		Event
			At MoneyAnchor
			Type Local
			EName RotatorAnchor
			BhvrOverride
				PyrRotateJitter 90 90 90
				Spin 0 0.1 0
				SpinJitter 0 0.015 0
				StartJitter 1 2 1
			End
		End

		Event
			At RotatorAnchor
			Type Local
			EName MoneyGeo
			Bhvr Auras\Male\Wealthy\moolah.bhvr
			BhvrOverride
				#EndScale 0.065 0.0550 0.0550
				PositionOffset 1 1 1
				PyrRotate 0 90 0
				Spin 0.05 0 0.1
				TintGeom 1
				#FadeOutLength 60
				#StartColor 255 255 255
				#HueShift 0
				#HueShiftJitter 0
				Scale 0.015 0.015 0.015
				Gravity 0.001
			End
			Geom FX_Money_01
			Geom FX_Money_02
			Geom FX_Money_03
			LifeSpan 80
		End

	End

End
