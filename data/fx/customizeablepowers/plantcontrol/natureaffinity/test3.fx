FXInfo

	Condition
		On time
		time 0

		Event
			At LArmR
			type Local
			Geom FX_Kelp_Wreath_LegL
			BhvrOverride
				PyrRotate	0 15 -5
				PositionOffset	0.7 0 0.1
			End
		End

		Event
			At LArmR
			type Local
			Geom FX_Kelp_Wreath_LegL
			BhvrOverride
				PyrRotate	180 -0 -15
				PositionOffset	0.9 0 0.0
			End
		End

		Event
			At LarmR
			type Local
			Geom FX_NatureBlast_Flower_Head
			BhvrOverride
				#PyrRotate	180 -0 -15
				PositionOffset	1.5 0 0.0
				Scale		2 .75 .75
			End
		End
	End

End