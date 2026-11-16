#
FxInfo


###########################################################

	Condition
		On TriggerBits
		TriggerBits MALE
		DoMany 1

		Event
			EName MaleAura
			At Root
			Type Local
			Flags	OneAtATime
			ChildFx :Shrubbery_NoIdle_CHILD.fx
			While MALE
		End

	End

	Condition
		On TriggerBits
		TriggerBits FEMALE
		DoMany 1

		Event
			EName FemaleAura
			At Root
			Type Local
			Flags	OneAtATime
			ChildFx :Shrubbery_NoIdle_CHILD.fx
			While FEMALE
		End

	End

	Condition
		On TriggerBits
		TriggerBits HUGE
		DoMany 1

		Event
			EName HugeAura
			At Root
			Type Local
			Flags	OneAtATime
			ChildFx :Shrubbery_NoIdle_CHILD.fx
			While HUGE
		End

	End

	Condition
		On TriggerBits
		TriggerBits FORWARD
		DoMany 1

		Event
			At Root
			Type Local
			ChildFx :Shrubbery_Trail_CHILD.fx
			EName Cloud
			Flags OneAtATime
			While FORWARD
		End

	End

	Condition
		On TriggerBits
		TriggerBits BACKWARD
		DoMany 1

		Event
			At Root
			Type Local
			ChildFx :Shrubbery_Trail_CHILD.fx
			EName Cloud
			Flags OneAtATime
			While BACKWARD
		End

	End

	Condition
		On TriggerBits
		TriggerBits STEPLEFT
		DoMany 1

		Event
			At Root
			Type Local
			ChildFx :Shrubbery_Trail_CHILD.fx
			EName Cloud
			Flags OneAtATime
			While STEPLEFT
		End

	End

	Condition
		On TriggerBits
		TriggerBits STEPRIGHT
		DoMany 1

		Event
			At Root
			Type Local
			ChildFx :Shrubbery_Trail_CHILD.fx
			EName Cloud
			Flags OneAtATime
			While STEPRIGHT
		End

	End

End
