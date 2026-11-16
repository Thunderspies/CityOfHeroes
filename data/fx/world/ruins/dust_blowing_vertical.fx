#
FxInfo

	Flags DontSuppress

	Input
		InpName Origin
	End


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName FogAnchor
			BhvrOverride
				PositionOffset 0 25 0
				PyrRotate 0 0 0
				Scale 2 1 1
				Alpha 255
				StartColor 234 224 178
			End
			Geom Dust_Plane_02
		End

	End

End
