#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			EName MoneyAnchor

			At HandR #Origin
			Type Start
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			#LifeSpan 90
		End

		Event
			EName RotatorAnchor

			At MoneyAnchor
			Type Local
			BhvrOverride
				PyrRotateJitter 90 90 90
				Spin 0 0.1 0
				SpinJitter 0 0.015 0
			End
		End

	End

	Condition
		On Time
		Time 0

		Event
			At RotatorAnchor
			Type Local
			EName MoneyGeo
			Bhvr :Moolah_Small.bhvr
			BhvrOverride
				#EndScale 0.065 0.0550 0.0550
				PositionOffset .4 .4 .4
			#	StartJitter	2 2 2
				PyrRotate 0 90 0
				Spin 0.05 0 0.1
				#FadeOutLength 15
				TintGeom 1
				FadeOutLength 60
				#StartColor 255 255 255
				#HueShift 0
				#HueShiftJitter 0
				Scale 0.012 0.012 0.012
				#Gravity 0.0001
			End
			Geom FX_Money_01
			Geom FX_Money_02
			Geom FX_Money_03
			LifeSpan 50
		End

	End

End
