#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Start
			ChildFx :Butterflies_Idle_Child01.fx
			EName Idle
			LifeSpan 0
		End

		Event
			At Hips
			Type Local
			ChildFx :Butterflies_Motion_Child.fx
			EName Go
			LifeSpan 0
		End

	End

End
