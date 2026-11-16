#
FxInfo


###########################################################

	Condition
		On TriggerBits
		TriggerBits MALE
		DoMany 1

		Event
			At Root
			Type Local
			EName NoFlyA
			Flags OneAtATime
			Until FLY
			CHILDFX	:Floral_NoIdle_CHILD2.fx
		End

	End

	Condition
		On TriggerBits
		TriggerBits FEMALE
		DoMany 1

		Event
			At Root
			Type Local
			EName NoFlyB
			Flags OneAtATime
			Until FLY
			CHILDFX	:Floral_NoIdle_CHILD2.fx
		End

	End

	Condition
		On TriggerBits
		TriggerBits HUGE
		DoMany 1

		Event
			At Root
			Type Local
			EName NoFlyC
			Flags OneAtATime
			Until FLY
			CHILDFX	:Floral_NoIdle_CHILD2.fx
		End

	End

End
