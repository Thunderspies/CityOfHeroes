#############################################################
## TouchOfDeath_Subtractive.fx
#############################################################

FxInfo
LifeSpan	55

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	UarmL
		Sound DClifedrain4 70 70 .86
	End
End



Condition
	On 	Time
	Time 	23

	Event

		ENAME	skull
		Type	local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\DarkMelee\TouchOfFearHandTendril.part
		LifeSpan	31
	End

End

Condition
	On 	Time
	Time 	13

	Event
		EName 	Hand1
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\DarkMelee\FearRing_subtractive.Part
		LifeSpan	55
	End

End

Condition
	On 	Time
	Time 	33

	Event
		EName 	Hand1
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\DarkMelee\FearSkullrays.part

		LifeSpan	25
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Hand1
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\DarkMelee\FearRing2_Subtractive.Part
		LifeSpan	55
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsA.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsB.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands_Additive.part

		Part	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part

		Part	CustomizeablePowers\DarkMelee\DarkHandsB.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsa.part

		PMagnet	LarmL
		LifeSpan	50
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkMelee\DarkHands.part
		Part	CustomizeablePowers\DarkMelee\DarkHands2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands_Additive.part

		Part	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part


		Part	CustomizeablePowers\DarkMelee\DarkHandsB.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsa.part

		PMagnet	WepL
		LifeSpan	50
	End

End


End


