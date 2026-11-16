#############################################################
## ShadowPunch.fx
#############################################################

FxInfo
LifeSpan 15

#############################################################

Input
	Inpname	Hips
End

##########################################################################
##Audio
##################################################################
Condition
	On	Time
	Time	14

	Event
		Type	Local
		At 	LarmL
		Sound Shadowpunch 50 50 .9
	End
End

##########################################################################
##Arms
##################################################################

Condition
	On	Time
	Time	0


	Event
		EName	10
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
	End

	Event
		EName	15
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
	End

	Event
		EName	16
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
	End

End



End

