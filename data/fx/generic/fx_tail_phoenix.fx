#
FxInfo

Flags InheritAlpha DontSuppress

###########################################################


	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName TailAnchor
			BhvrOverride
				PositionOffset 0 0 -0.36
				TintGeom 1
				#StartColor 255 120 0
			End
			Geom FX_PhoenixTail
		End

		Event
			AltPiv 1
			At TailAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 40 0 0
			End
			Part GENERIC\PhoenixFire_Out01.part
			Part GENERIC\PhoenixFire_Out02.part
		End

End
