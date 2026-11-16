#########################################################
##	Thunder Kick
FxInfo

Lifespan 100

##################################

Condition
	On	Time
	Time	0

	Event
		ENAME	KickFlash
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr

		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive.part

		Part1	CustomizeablePowers\DarkMelee\DarkHandsA.part
		Part2	CustomizeablePowers\DarkMelee\DarkHandsB.part
		Part3	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part
		Part1	CustomizeablePowers\DarkMelee\DarkHand.part
		Part3	CustomizeablePowers\DarkMelee\DarkHands.part
		Sound darkwindup3 60 60 .44
		LifeSpan 50

	End

	Event
		ENAME	KickFlash
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr

		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive.part

		Part1	CustomizeablePowers\DarkMelee\DarkHandsA.part
		Part2	CustomizeablePowers\DarkMelee\DarkHandsB.part
		Part3	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part
		Part1	CustomizeablePowers\DarkMelee\DarkHand.part
		Part3	CustomizeablePowers\DarkMelee\DarkHands.part
		LifeSpan 50

	End
End

##################################

End