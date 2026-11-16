#########################################################
##	chill touch hit
FxInfo

#LifeSpan	150

##################################
Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	FootR
		Sound poisongas2_loop 50 50 .18
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End




Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	2
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	15
	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	3
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet Knee1
		LifeSpan	15

	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	4
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet FootL
		LifeSpan	15
	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	5		
		Type	Local
		At	FootL

		Bhvr	Behaviors\GenericParticleFade.bhvr

		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	15
	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	6
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet Kneer
		LifeSpan	15

	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	7
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet FootR
		LifeSpan	15
	End


End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	9
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	15
	End

##########################################################################
##Arm
##################################################################

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	10
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet ElbowL
		LifeSpan	15

	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	11
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet HandL
		LifeSpan	15
	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	12
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	15
	
	End


End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	13
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet ElbowL
		LifeSpan	15

	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	14
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		PMagnet HandL
		LifeSpan	15
	End

End

Condition
	On	cycle
	Time	5
	Chance	.25

	Event
		EName	15
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont.part
		Part	CustomizeablePowers\Poison\GlowTrailLightCont2.part
		Part	CustomizeablePowers\Poison\GlowTrailBlackCont2.part
		Part	CustomizeablePowers\Poison\GlowstarsTrailCont2.part
		Part	CustomizeablePowers\Poison\GlowBubblesTrailCont2.part
		LifeSpan	15
	
	End



End

End				