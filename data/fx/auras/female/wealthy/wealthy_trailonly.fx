#
FxInfo


###########################################################

	Condition
		On TriggerBits
		TriggerBits FORWARD
		DoMany 1

		Event
			At Hips
			Type Start
			ChildFx :Wealthy_Trail_Child01.fx
			EName MoneyTrail
			Flags OneAtATime
			While FORWARD
		End

	End

	Condition
		On TriggerBits
		TriggerBits BACKWARD
		DoMany 1

		Event
			At Hips
			Type Start
			ChildFx :Wealthy_Trail_Child01.fx
			EName MoneyTrail
			Flags OneAtATime
			While BACKWARD
		End

	End

	Condition
		On TriggerBits
		TriggerBits STEPLEFT
		DoMany 1

		Event
			At Hips
			Type Start
			ChildFx :Wealthy_Trail_Child01.fx
			EName MoneyTrail
			Flags OneAtATime
			While STEPLEFT
		End

	End

	Condition
		On TriggerBits
		TriggerBits STEPRIGHT
		DoMany 1

		Event
			At Hips
			Type Start
			ChildFx :Wealthy_Trail_Child01.fx
			EName MoneyTrail
			Flags OneAtATime
			While STEPRIGHT
		End

	End

End
