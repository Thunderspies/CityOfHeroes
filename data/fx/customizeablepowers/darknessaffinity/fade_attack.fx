#                                                          
FxInfo

	LifeSpan 45


###########################################################

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Start
			EName BlackHole
			Part CustomizeablePowers\DarknessAffinity\Buildup_Core_Continuing.part
			Part CustomizeablePowers\DarknessAffinity\Buildup_Core.part
			LifeSpan 40
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
			EName Tendrils
			Part CustomizeablePowers\DarknessAffinity\Buildup_Tendrils.part
			Part CustomizeablePowers\DarknessAffinity\Buildup_Tendrils_Subtractive.part
			Part CustomizeablePowers\DarknessAffinity\Buildup_Tendrils_Subtractive_Flat.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Root
			Type Start
			EName Tendrils
			Part CustomizeablePowers\DarknessAffinity\Buildup_Tendrils_Flat.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 44

		Event
			At Root
			Type Start
			EName Shockwave
			Part CustomizeablePowers\DarknessAffinity\Explosion_Shockwave.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 45

		Event
			At Root
			Type Start
			EName Shockwave
			Part CustomizeablePowers\DarknessAffinity\Explosion_Swirl.part
			LifeSpan 10
		End

	End

End
