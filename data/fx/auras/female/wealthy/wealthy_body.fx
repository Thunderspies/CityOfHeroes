#
FxInfo


###########################################################

	Condition
		On TriggerBits
		TriggerBits MALE
		DoMany 1

		Event
			At Hips
			Type Local
			ChildFx :Wealthy_Idle_Child01.fx
			EName MoneyTrail
			Flags OneAtATime
			While MALE
		End

	End

	Condition
		On TriggerBits
		TriggerBits FEMALE
		DoMany 1

		Event
			At Hips
			Type Local
			ChildFx :Wealthy_Idle_Child01.fx
			EName MoneyTrail
			Flags OneAtATime
			While FEMALE
		End

	End

	Condition
		On TriggerBits
		TriggerBits HUGE
		DoMany 1

		Event
			At Hips
			Type Local
			ChildFx :Wealthy_Idle_Child01.fx
			EName MoneyTrail
			Flags OneAtATime
			While HUGE
		End

	End



###########################################################


End
