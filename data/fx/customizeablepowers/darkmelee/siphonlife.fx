#############################################################
## SiphonLife.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\DarkMelee\DarkBlastTenticle.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsEmber_Additive.part
		Sound Breathattack 70 70 .8
		PMagnet	LarmL
		Lifespan 60
	End

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part	CustomizeablePowers\DarkMelee\DarkBlastTenticle.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part
		Part	CustomizeablePowers\DarkMelee\DarkHandsEmber_Dark.part
		PMagnet	WepL
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName 	HandGlow
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\DarkMelee\DarkDrainGlow.part
		Lifespan 39
	End
End

#############################################################

End