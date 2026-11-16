#
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	EnvironmentalModification_01 100 100 .8
	End
End

	Condition
		On TriggerBits
		TriggerBits MALE
		DoMany 1

		Event
			EName MaleAura
			At Root
			Type Local
			Flags	OneAtATime
			ChildFx :BioArmor_02_Male_CHILD.fx
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
			ChildFx :BioArmor_02_Fem_CHILD.fx
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
			ChildFx :BioArmor_02_Huge_CHILD.fx
			While HUGE
		End

	End


End
