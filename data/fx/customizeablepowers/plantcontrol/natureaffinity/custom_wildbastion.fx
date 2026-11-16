#
FxInfo

	LifeSpan 150
Input
	InpName	Hips
End

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	NA_WildBastion_01 100 100 .8
	End
End

	Condition
		On Triggerbits
		Triggerbits MALE
		DoMany 1

		Event
			Ename MANCANDY
			At root
			Type Local
			Flags OneAtATime
			While MALE
			ChildFX :Custom_IvyBody_Male_Activation.fx
		End
	End

	Condition
		On Triggerbits
		Triggerbits FEMALE
		DoMany 1

		Event
			Ename FEMALECANDY
			At root
			Type Local
			Flags OneAtATime
			While FEMALE
			ChildFX :Custom_IvyBody_Female_Activation.fx
		End
	End

	Condition
		On Triggerbits
		Triggerbits HUGE
		DoMany 1

		Event
			Ename HUGECANDY
			At root
			Type Local
			Flags OneAtATime
			While HUGE
			ChildFX :Custom_IvyBody_Huge_Activation.fx
		End
	End

##################################################################################

#######################################################

End

