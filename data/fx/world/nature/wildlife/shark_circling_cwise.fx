#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName SharkAnchor
			BhvrOverride
				PositionOffset 0 -0.5 5
				Spin 0 0.0080 0
			End
		End

		Event
			Anim FX_SharkFin
			At SharkAnchor
			Type Local
			EName SharkFin_1
			BhvrOverride
				PositionOffset 20 0 0
				PyrRotate 0 180 0
			End
		End

		Event
			At SharkAnchor
			Type Local
			EName Ripples
			BhvrOverride
				PositionOffset 20 -0.1 -2
				PyrRotate 0 180 0
			End
			Part WORLD\Nature\Wildlife\RippleSplash.part
			Sound 	SharkCircleVFX_Loop 50 50 .2
		End

		Event
			At SharkAnchor
			Type Local
			EName Ripples
			BhvrOverride
				PositionOffset 20 0.4 -1.2
				PyrRotate 0 180 0
			End
			Part WORLD\Nature\Wildlife\RippleSplash_Flat.part
			Part WORLD\Nature\Wildlife\RippleSplashFoam_Flat.part
		End

	End

End