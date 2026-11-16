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
				PositionOffset 0 1.5 0
				#PyrRotate 0 270 0
				Scale 2 1 1.2
				Alpha 180
				StartColor 234 224 178
			End
			Geom Dust_Plane_01
		End
	End

End
