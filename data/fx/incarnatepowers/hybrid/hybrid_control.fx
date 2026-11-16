#
FxInfo

	LifeSpan 1000


###########################################################
Condition
	On 	Time
	Time	25
	Event
		Type	Local
		At	Root
		Sound 	Hybrid_Control 100 100 .8
	End
End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			ChildFx IncarnatePowers/Incarnate_Gather.fx
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			Part :Lingering_Cloud_Out.part
			LifeSpan 50
		End

	End

	Condition
		On Time
		Time 18

		Event
			At Root
			Type Local
			ChildFX :Hybrid_Control_Child.fx
			LifeSpan 70
		End
	End

	Condition
		On Cycle
		Time 150

		Event
			At Root
			Type Local
			ChildFX :Hybrid_Control_Child.fx
			LifeSpan 70
		End
	End


End
