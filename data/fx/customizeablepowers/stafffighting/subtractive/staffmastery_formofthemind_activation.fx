#
FxInfo

	LifeSpan 55


###########################################################

	Condition
		On TRIGGERBITS
		Triggerbits	2HLARGE
		Domany 1

		Event
			Ename WorkIt
			At Root
			Type Local
			flags OneAtATime
			ChildFX :CHILD_Staffmastery_FormOfTheMind_Activation.fx
		End
	End


End
