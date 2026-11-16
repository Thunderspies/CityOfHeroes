#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName anchor
			BhvrOverride
				PositionOffset 0 0 0
				Scale 1.1 0.7 1.1
			End
			Geom WarningLight_Outer
		End

		Event
			At anchor
			Type Local
			Geom WarningLight_Glass
		End

		Event
			At anchor
			Type Local
			EName SPINANCHOR
			BhvrOverride
				PositionOffset 0 -0.03 0
				Spin 0 0.2 0
			End
			Geom WarningLight_Inner
		End

		Event
			At anchor
			Type Local
			BhvrOverride
				PositionOffset 0 -0.03 0
				Spin 0 0.2 0
				StartColor 120 120 30
				#Scale 0.6 0.6 0.6
				Alpha 120
			End
			Geom WarningLight_Beam
		End

		Event
			At anchor
			Type Local
			BhvrOverride
				PositionOffset 0 -0.03 0
				Spin 0 0.2 0
				StartColor 120 120 30
				#Scale 0.6 0.6 0.6
				Alpha 120
			End
			Geom WarningLight_Beam
			Part WORLD/City/WarningLight_Highlight.part
		End

#		Event
#			At SPINANCHOR
#			Type Local
#			BhvrOverride
#				PositionOffset 0.3 0.57 0
#				PyrRotate 0 90 90
#			End
#			Part WORLD/City/WarningLight_MainHighlight.part
#		End
#
#		Event
#			At SPINANCHOR
#			Type Local
#			BhvrOverride
#				PositionOffset -0.3 0.57 0
#				PyrRotate 0 90 -90
#			End
#			Part WORLD/City/WarningLight_MainHighlight.part
#		End

		Event
			At SpinAnchor
			Type Local
			BhvrOverride
				PositionOffset 4 0.57 0
			End
			Part WORLD/City/WarningLight_Highlight.part
			Part WORLD/City/WarningLight_Highlight.part
			#Geom geo_Fx_TestSphere
		End

		Event
			At SpinAnchor
			Type Local
			BhvrOverride
				PositionOffset -4 0.57 0
			End
			Part WORLD/City/WarningLight_Highlight.part
			Part WORLD/City/WarningLight_Highlight.part
			#Geom geo_Fx_TestSphere
		End

	End

End
