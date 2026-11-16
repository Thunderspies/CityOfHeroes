#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			Ename SpinAnchor
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 -0.2 -1
				Spin 0 0 0.03
				Alpha 1
				Scale .04 .04 .04
			End
			geom FX_Offset_Array7
		End

		Event
			At SpinAnchor
			Type Local
			EName Prop1
			Altpiv 1
			ChildFX :HoodedLamps_Child.fx
		End

		Event
			At SpinAnchor
			Type Local
			EName Prop2
			Altpiv 2
			ChildFX :HoodedLamps_Child.fx
		End

		Event
			At SpinAnchor
			Type Local
			EName Prop3
			Altpiv 3
			ChildFX :HoodedLamps_Child.fx
		End

		Event
			At SpinAnchor
			Type Local
			EName Prop4
			Altpiv 4
			ChildFX :HoodedLamps_Child.fx
		End

		Event
			At SpinAnchor
			Type Local
			EName Prop5
			Altpiv 5
			ChildFX :HoodedLamps_Child.fx
		End

		Event
			At SpinAnchor
			Type Local
			EName Prop6
			Altpiv 6
			ChildFX :HoodedLamps_Child.fx
		End

		Event
			At SpinAnchor
			Type Local
			EName Prop7
			Altpiv 7
			ChildFX :HoodedLamps_Child.fx
		End

End
