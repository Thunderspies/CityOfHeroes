#                                                          
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Cycle
		Time 10

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\Shade_Scale_Small.bhvr
			BhvrOverride
				PositionOffset 0 0.1 0
				PyrRotate 0 0 0
				PyrRotateJitter 10 10 30
				EndScale 0.2 0.2 0.2
				StartJitter 0.1 0.25 0.1
			End
			Part CustomizeablePowers\DarknessControl\Head_Core_Solid.part
			Geom Tintable_DarkGrasp
			LifeSpan 15
		End

	End

End
