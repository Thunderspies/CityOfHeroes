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

		#Event
			#At Root
			#Type Local
			#EName FogAnchor
			#BhvrOverride
				#PositionOffset 0 1.5 0
				#PyrRotate 0 270 0
				#Scale 1 1 1
				#Alpha 200
				#StartColor 234 224 178
			#End
			#Geom Dust_Plane_01
		#End

		Event
			At Root
			Type Local
			EName DustBalls
			BhvrOverride
				PositionOffset -5 2 0
				#PyrRotate 0 0 0
				#Alpha 0
				#StartColor 255 255 255
			End
			Part WORLD\Ruins\Fog_EerieGroundFog.part
		End

	End

End
