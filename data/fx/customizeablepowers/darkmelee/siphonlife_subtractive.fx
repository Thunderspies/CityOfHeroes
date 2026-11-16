#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

#########################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\DarkMelee\DarkBlastTenticle_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastTenticle.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part
		Sound Breathattack 70 70 .8
		PMagnet	LarmL
	End

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Bhvr	behaviors\GenericParticleFade.bhvr

		Part	CustomizeablePowers\DarkMelee\DarkBlastTenticle_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastTenticle.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part
		PMagnet	WepL
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName 	HandGlow
		Type	Local
		At	WepL
		Part	CustomizeablePowers\DarkMelee\DarkDrainGlow.part
	End
End


End


