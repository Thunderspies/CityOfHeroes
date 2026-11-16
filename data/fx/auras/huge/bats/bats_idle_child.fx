#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName BatAnchor
			Flags OneAtATime
			LifeSpan 200
		End

	End

	Condition
		On Time
		Time 0

		Event
			At BatAnchor
			Type Local
			EName RotatorAnchor
			BhvrOverride
				EndScale 1 1 1
				PyrRotateJitter 90 0 0
				Spin 0 0.1 0
				SpinJitter 0 0.015 0
				StartJitter 1 1 1
			End
		End

		Event
			Anim FX_Bat_01
			At RotatorAnchor
			Type Local
			EName BatGeo
			Bhvr behaviors/Scale_Bats.bhvr
			BhvrOverride
				EndScale 0.08 0.08 0.08
				PositionOffset 1 1 1
				PyrRotate 0 135 90
				Spin 0 0 0.1
				FadeOutLength 60
			End
			LifeSpan 80
		End

	End

End
