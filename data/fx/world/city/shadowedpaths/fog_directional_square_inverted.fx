#                                                          
FxInfo

	Flags DontSuppress

	Input
		InpName Origin
	End

	LifeSpan 0


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 2 -8
				PyrRotate 0 270 0
			End
			Part WORLD\City\ShadowedPaths\Fog_Directional_Square_Inverted.part
		End

	End

End
