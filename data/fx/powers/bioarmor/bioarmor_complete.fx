#
FxInfo

Flags    InheritAlpha InheritGeoScale

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
			ChildFx :BioArmor_Male_Complete.fx
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
			ChildFx :BioArmor_Fem_Complete.fx
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
			ChildFx :BioArmor_Huge_Complete.fx
			While HUGE
		End

	End


End
