#############################################################
## NoxiousGasContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	FootR
		bhvr	behaviors\soundFade3.bhvr
		Sound poisongas2_loop 50 50 0.18
		Flags	PowerLoopingSound
		Lifespan 220
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	2
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	3
		Type	Local
		At	ULEGL
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet Knee1
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	4
		Type	Local
		At	LLEGL
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet FootL
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	5
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	6
		Type	Local
		At	ULEGR
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet Kneer
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	7
		Type	Local
		At	LLEGR
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet FootR
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	9
		Type	Local
		At	Hair
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	10
		Type	Local
		At	UArmL
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet ElbowL
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	11
		Type	Local
		At	LArmL
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet HandL
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	12
		Type	Local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	13
		Type	Local
		At	UArmR
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet ElbowL
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	14
		Type	Local
		At	LArmR
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		PMagnet HandL
		Lifespan 15
	End
End

Condition
	On	cycle
	Time	5
	Chance	0.25

	Event
		EName	15
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.Bhvr
		Part	:GlowTrailBlackCont.part
		Part	:GlowTrailLightCont.part
		Part	:GlowBubblesTrailCont.part
		Part	:GlowTrailLightCont2.part
		Part	:GlowTrailBlackCont2.part
		Part	:GlowstarsTrailCont2.part
		Part	:GlowBubblesTrailCont2.part
		Lifespan 15
	End
End

#############################################################

End