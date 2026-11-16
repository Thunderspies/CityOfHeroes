FxInfo
Lifespan 200

	Condition
		On Time
		Time 0

		Event
			At LArmR
			Type Local
			Bhvr :BladeFade.bhvr
			BhvrOverride
				PositionOffset 0.9 0.1 0.05
				PyrRotate 5 -100 0
				Scale 0.8 0.8 0.8
			End
			Geom PsionicCrystalBlade
		End

		Event
			At LArmL
			Type Local
			Bhvr :BladeFade.bhvr
			BhvrOverride
				PositionOffset -0.9 0.1 0.05
				PyrRotate 5 100 0
				Scale 0.8 0.8 0.8
			End
			Geom PsionicCrystalBlade
		End
	End

End