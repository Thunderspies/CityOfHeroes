#
FxInfo


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Bats_Scattered_Loop 100 100 .1
	End
End


	Condition
		On Cycle
		Time 10

		Event
			At Root
			Type Start
			ChildFx :Bats_Scattered_CHILD.fx
			EName Bats
			LifeSpan 150
		End

	End

End
