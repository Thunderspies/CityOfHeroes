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
			EName FogAnchor
			BhvrOverride
				PositionOffset 0 0.5 0
				PyrRotate 0 270 0
				Scale 1.5 1 2.5
				Alpha 200
				StartColor 230 230 190
			End
			Geom FX_Fog_Plane_Narrow
		End

		Event
			At FogAnchor
			Type Local
			BhvrOverride
				PositionOffset -5.0 2.0 0.0
			End
			Part WORLD\City\ShadowedPaths\Fog_Directional_Strip_Narrow_Inverted.part
		End

	End

End
