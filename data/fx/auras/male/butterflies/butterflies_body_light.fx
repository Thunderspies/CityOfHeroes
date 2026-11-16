#
FxInfo


###########################################################

	Condition
		On Time
		time	0

		Event
			At Hips
			Type Start
			ChildFx :Butterflies_Light_Idle_Child01.fx
			EName Idle
			BhvrOverride
				#Scale 1 1 1
			End
			LifeSpan 0
		End

		Event
			At Hips
			Type Local
			ChildFx :Butterflies_Light_Motion_Child.fx
			EName Go
			BhvrOverride
				Scale 1 1 1
			End
			LifeSpan 0
		End

	End

End
