#                                                          
FxInfo

	LifeSpan 90


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName Shockwave
			Part CustomizeablePowers\DarknessAffinity\Tendrils_Core_Flat_Subtractive.part
			Part CustomizeablePowers\DarknessAffinity\Tendrils_Core_Flat.part
			LifeSpan 15
		End

		Event
			At WepR
			Type Local
			EName HandR
			Part CustomizeablePowers\DarknessAffinity\DarkHand_Movement.part
			Part CustomizeablePowers\DarknessAffinity\DarkHandsEmber_Movement.part
			LifeSpan 80
		End

		Event
			At WepL
			Type Local
			EName HandL
			Part CustomizeablePowers\DarknessAffinity\DarkHand_Movement.part
			Part CustomizeablePowers\DarknessAffinity\DarkHandsEmber_Movement.part
			LifeSpan 80
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Start
			EName Shockwave
			Part CustomizeablePowers\DarknessAffinity\Tendrils_Growing.part
			Part CustomizeablePowers\DarknessAffinity\Tendrils_Growing2.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Start
			EName Shockwave
			Part CustomizeablePowers\DarknessAffinity\Tendrils_Shriking.part
			Part CustomizeablePowers\DarknessAffinity\Tendrils_Core_Flat_Shrinking.part
			Part CustomizeablePowers\DarknessAffinity\Tendrils_Core_Flat_Shrinking_Subtractive.part
			Part CustomizeablePowers\DarknessAffinity\Rings_Shrinking.part
			LifeSpan 55
		End

	End

End
