#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan 1800

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Arrow
		Type	Local
		At	chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:FlashGlows.part
		Sound arrowhit2 70 70 .7
	End

	Event
		EName	Arrow
		Type	Local
		At	chest
		Part	:FlashGlow.part
		LifeSpan 6
	End

	Event
		EName	Arrow
		Type	Local
		At	chest
		Part	:FlashHitLightning.part
		Part	:FlashHitLightningSmall.part
		LifeSpan 5
	End
End

#############################################################

End