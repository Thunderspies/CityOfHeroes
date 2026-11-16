#
FxInfo

	LifeSpan 175


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_Fade_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Start
			EName BlackHole
			Part :Buildup_Core_Continuing.part
			Part :Buildup_Core.part
			LifeSpan 40
		End

		Event
			At WepR
			Type Local
			EName HandR
			Part POWERS\DarknessControl\DarkHandsEmber_Moving.part
			Part POWERS\DarknessControl\DarkHand_Moving.part
			LifeSpan 80
		End

		Event
			At WepL
			Type Local
			EName HandL
			Part POWERS\DarknessControl\DarkHandsEmber_Moving.part
			Part POWERS\DarknessControl\DarkHand_Moving.part
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
			Part :Buildup_Tendrils.part
			Part :Buildup_Tendrils_Subtractive.part
			Part :Buildup_Tendrils_Subtractive_Flat.part
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
			Part :Buildup_Tendrils_Flat.part
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
			Part POWERS\DarknessControl\Explosion_Swirl.part
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
			Part POWERS\DarknessControl\Explosion_Shockwave.part
			LifeSpan 10
		End

	End

End
