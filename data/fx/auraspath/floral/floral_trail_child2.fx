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
			Until AIR
			CHILDFX	:Floral_Trail_GRANDCHILD.fx
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
			Until AIR
			CHILDFX	:Floral_Trail_GRANDCHILD.fx
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
			Until AIR
			CHILDFX	:Floral_Trail_GRANDCHILD.fx
		End

	End

End
