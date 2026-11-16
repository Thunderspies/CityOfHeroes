#                                                          
FxInfo

	LifeSpan 80


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound BR_Aim_Activate_01 100 100 0.25
		End

	End

	Condition
		On Time
		Time 0

		Event
			At C_Chest
			Type Local
			ChildFx :Child_Aim.fx
			EName WeaponOffset
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName Pow2
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :YellowUpGlow.part
			Part :YellowUpSpecks2.part
			#Sound clawYellow3 100 30 0.8
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Chest
			Type Local
			EName Pow
			Bhvr Behaviors\GenericParticleFade.bhvr
			#Part :YellowUpSpecks.part
			Part :YellowUpRing01.part
			Part :YellowUpRing01a.part
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Chest
			Type Local
			EName Pow
			Bhvr Behaviors\GenericParticleFade.bhvr
			#Part :YellowUpSpecks.part
			Part :YellowUpRing02.part
			Part :YellowUpRing02a.part
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Chest
			Type Local
			EName Pow
			Bhvr Behaviors\GenericParticleFade.bhvr
			#Part :YellowUpSpecks2.part
			Part :YellowUpRingSlow.part
			Part :YellowUpRing03.part
			Part :YellowUpRing03a.part
		End

	End

	Condition
		On Time
		Time 13

		Event
			Type Destroy
			EName pow2
		End

	End

	Condition
		On Time
		Time 75

		Event
			Type Destroy
			EName all
		End

	End

End
