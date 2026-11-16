#########################################################
##	template

FxInfo


LifeSpan 30

Condition
	On 	Time
	Time 	0

	Event
		EName	Arrow
		Type	Local
		At	chest

		BHVR	Behaviors/GenericParticleFade.bhvr
		
		Part	:FlashGlows.part

		Sound arrowhit2 70 70 .7
		LifeSpan 30
	End

	Event
		EName	Arrow
		Type	Local
		At	chest

		Part	:FlashGlow.part

		LifeSpan 12
	End
	
	Event
		EName	Arrow
		Type	Local
		At	chest

		Part	:FlashHitLightning.part
		Part	:FlashHitLightningSmall.part

		LifeSpan 10
	End

End

End		