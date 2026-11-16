#
FxInfo

	LifeSpan 50


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
			ChildFX :CHILD_Staffmastery_FormOfTheBody_Activation.fx
		End
	End

End
