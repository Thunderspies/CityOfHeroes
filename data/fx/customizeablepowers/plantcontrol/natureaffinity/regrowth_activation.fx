#
FxInfo
Lifespan 400

############################################################


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
			ChildFX :Regrowth_Body_MALE.fx
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
			ChildFX :Regrowth_Body_FEMALE.fx
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
			ChildFX :Regrowth_Body_HUGE.fx
		End
	End
End