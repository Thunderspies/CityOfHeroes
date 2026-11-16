#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			Ename BeginAnchor
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 -0.2 -1
				Scale .5 .5 .5
			End
		End

		Event
			Ename SpinAnchor
			At BeginAnchor
			Type Local
			BhvrOverride
				Spin 0 0 0.03
			End
		End

		Event
			At SpinAnchor
			Type Local
			EName PropAnchor
			BhvrOverride
				PositionOffset 2 3 0
			End
			ChildFX :HoodedLamps_Child.fx
			#geom Geo_FX_TestSphere
		End

		Event
			At SpinAnchor
			Type Local
			EName PropAnchor
			BhvrOverride
				PositionOffset 2 -3 0
			End
			ChildFX :HoodedLamps_Child.fx
			#geom Geo_FX_TestSphere
		End

		Event
			At SpinAnchor
			Type Local
			EName PropAnchor
			BhvrOverride
				PositionOffset -2 3 0
			End
			ChildFX :HoodedLamps_Child.fx
			#geom Geo_FX_TestSphere
		End

		Event
			At SpinAnchor
			Type Local
			EName PropAnchor
			BhvrOverride
				PositionOffset -2 -3 0
			End
			ChildFX :HoodedLamps_Child.fx
			#geom Geo_FX_TestSphere
		End

		Event
			At SpinAnchor
			Type Local
			EName PropAnchor
			BhvrOverride
				PositionOffset -4 -0 0
			End
			ChildFX :HoodedLamps_Child.fx
			#geom Geo_FX_TestSphere
		End

		Event
			At SpinAnchor
			Type Local
			EName PropAnchor
			BhvrOverride
				PositionOffset 4 -0 0
			End
			ChildFX :HoodedLamps_Child.fx
			#geom Geo_FX_TestSphere
		End

End
