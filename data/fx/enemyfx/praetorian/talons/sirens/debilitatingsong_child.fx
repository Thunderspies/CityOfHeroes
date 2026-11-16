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
			Bhvr ENEMYFX\Praetorian\Talons\Sirens\DebilitatingStream.bhvr
			BhvrOverride
				PositionOffset 0 0 0.3
				PyrRotate -60 0 0
				Scale 1 0.2 0.2
			End
			Geom FX_MusicStream
			LifeSpan 20
		End

	End

End
