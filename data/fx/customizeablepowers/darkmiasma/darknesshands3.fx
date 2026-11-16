#########################################################
## Darkness Hands - Used for "Darkest Night" powers
#########################################################

FxInfo

Lifespan 220

## SOUNDS #######################################################

Condition

	On	Time
	Time	0

	Event
		Type 	Local
		At	chest
		Sound miasma_loop 50.0 50.0 .4
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End

Condition

	On	Time
	Time	0

	Event
		Type 	Local
		At	chest
		Sound miasma_loop 50.0 50.0 .4
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		Sound miasmaburst 100 100 .8
		Lifespan	90
	End

	Event
		Type	Local
		At	WepL
		Sound miasma_loop 100 100 .5
		Lifespan	90
	End

	Event

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber_Alpha.part

		LifeSpan	90
	End

	Event
		ENAME	Base
		Type	local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber_Alpha.part

		LifeSpan	90
	End
End

#########################################################

End