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
			EName Straight
			BhvrOverride
				PositionOffset -50 0.5 10
				PyrRotate 0 180 0
				#Scale 1 1 1
				Alpha 255
				#StartColor 234 224 178
			End
			Geom FX_WaterEdgeRipple_Flat
		End

		Event
			At Root
			Type Local
			EName Corner
			BhvrOverride
				PositionOffset -50 0.5 10
				PyrRotate 0 180 0
				#Scale 1 1 1
				Alpha 255
				#StartColor 234 224 178
			End
			Geom FX_WaterEdgeRipple_Corner
		End

		Event
			At Root
			Type Local
			EName Wobbly
			BhvrOverride
				PositionOffset 70 0.5 0
				#PyrRotate 0 270 0
				#Scale 1 1 1
				Alpha 255
				#StartColor 234 224 178
			End
			Geom FX_WaterEdgeRipple_Uneven
		End

	End

End
