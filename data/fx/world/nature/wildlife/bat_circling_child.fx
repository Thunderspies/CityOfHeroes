#############################################################
## Bat_Circling_CHILD.fx
#############################################################

FxInfo

###########################################################

	Condition
		On Time
		Time 0

		Event
			At origin
			Type Local
			EName RotatorAnchor
			BhvrOverride
				PyrRotateJitter 90 0 0
				SpinJitter 0 0.05 0
			End
		End

		Event
			EName BatGeo
			At RotatorAnchor
			Type Local
			Bhvr	behaviors/Scale_Cards.bhvr
			BhvrOverride
				FadeOutLength	60
				PositionOffset 	20 0 0
				PyrRotate 	0 0 180
				EndScale 	0.1 0.1 0.1
				Spin 		0 0 0.05
			End
			Anim FX_Bat_01
			LifeSpan 90
		End

	End

###########################################################

End
