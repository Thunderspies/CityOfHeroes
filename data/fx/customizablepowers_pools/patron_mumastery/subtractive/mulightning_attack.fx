#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 7

		Event
			At Origin
			Type Local
			ChildFx :CHILD_LightningArc.fx
			Sound ThunderCrack2 80 80 0.89
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 12

		Event
			At Origin
			Type Local
			ChildFx :CHILD_LightningArc.fx
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 14

		Event
			At Origin
			Type Local
			ChildFx :CHILD_LightningArc.fx
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 16

		Event
			At Origin
			Type Local
			ChildFx :CHILD_LightningArc.fx
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Origin
			Type Local
			Sound lightningbolthit 100 100 0.7
			LifeSpan 40
		End

	End

End
