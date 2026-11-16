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
			ChildFx :BioArmor_02_Male_Minimal.fx
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
			ChildFx :BioArmor_02_Fem_Minimal.fx
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
			ChildFx :BioArmor_02_Huge_Minimal.fx
			While HUGE
		End

	End


End
