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
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	90
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

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	90
	End

	Event
		ENAME	Base
		Type	local
		At	WepL
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	90
	End
End

#########################################################

End