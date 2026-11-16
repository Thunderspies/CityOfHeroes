#
FxInfo

	Input
		InpName Hips
	End


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName Anchor
			BhvrOverride
				PyrRotateJitter 10 0 10
				Scale 0.2 0.6 0.2
				Spin 0 -0.1 0
				StartJitter 1 0 0
			End
		End

	End

	Condition
		On Time
		Time 21

		Event
			At Anchor
			Type Local
			Bhvr ENEMYFX\Praetorian\Talons\Sirens\SorrowContinuing.bhvr
			BhvrOverride
				Gravity 0.002
				PositionOffset 0 0 0
				Scale 0.2 0.3 0.2
				EndScale 1.2 1.2 1.2
			End
			Geom FX_MusicRing_04
			LifeSpan 1
		End

	End

End
