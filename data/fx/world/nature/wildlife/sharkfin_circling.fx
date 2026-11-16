#
FxInfo

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type StartPositOnly
			EName SharkAnchor
			BhvrOverride
				PositionOffset 0 -0.5 5
				Spin 0 -0.0080 0
			End
		End

		Event
			Anim FX_SharkFin
			At SharkAnchor
			Type Local
			EName SharkFin_1
			BhvrOverride
				PositionOffset 20 0 0
			End
		End

		Event
			At SharkAnchor
			Type Local
			EName Ripples
			BhvrOverride
				PositionOffset 20 -0.1 2
			End
			Part WORLD\Nature\Wildlife\RippleSplash.part
		End

		Event
			At SharkAnchor
			Type Local
			EName Ripples
			BhvrOverride
				PositionOffset 20 0.4 1.5
			End
			Part WORLD\Nature\Wildlife\RippleSplash_Flat.part
			Part WORLD\Nature\Wildlife\RippleSplashFoam_Flat.part
		End

	End

End
