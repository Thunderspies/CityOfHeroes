#############################################################
## MaulAttack.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part	CustomizeablePowers\DarkMelee\SmokeHands_Additive.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan 45
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part	CustomizeablePowers\DarkMelee\SmokeHands_Additive.part

		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan 45
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part	CustomizeablePowers\DarkMelee\SmokeHands_Additive.part

		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan 45

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part	CustomizeablePowers\DarkMelee\SmokeHands_Additive.part

		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan 45
	End

	Event
		EName	15
		Type	Local
		At	WepL
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part	CustomizeablePowers\DarkMelee\SmokeHands_Additive.part

		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan 45
	End

	Event
		EName	16
		Type	Local
		At	WepR
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part	CustomizeablePowers\DarkMelee\SmokeHands_Additive.part

		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan 45
	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		Bhvr	CustomizeablePowers\DarkMelee\speed.bhvr
		At	root
		BhvrOverride
			Alpha			150
			TintGeom		1
		End
		Anim	Tintable_MblurShadowFighting
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		Bhvr	CustomizeablePowers\DarkMelee\speed.bhvr
		At	root
		BhvrOverride
			Alpha			150

			TintGeom		1
		End
		Anim	Tintable_MblurShadowFighting
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		Bhvr	CustomizeablePowers\DarkMelee\speed.bhvr
		At	root
		BhvrOverride
			Alpha			150
			TintGeom		1
		End
		Anim	Tintable_MblurShadowFighting
		Sound Flurry 50 50 0.75
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		Bhvr	CustomizeablePowers\DarkMelee\speed.bhvr
		At	root
		BhvrOverride
			Alpha			150

			TintGeom		1
		End
		Anim	Tintable_MblurShadowFighting
	End
End

#############################################################

End