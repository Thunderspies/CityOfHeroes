#############################################################
## NinjaPsionicsDefence.fx
#############################################################

FxInfo
Lifespan 80

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound birthfx 70 70 .7
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	19

	Event
		ENAME	hookup
		Type	local
		At	Root
		Bhvr	CustomizeablePowers\Ninjitsu\PsionicPowerHandOffset.bhvr

	End

	Event
		ENAME	HandGlow
		Type	local
		At	hookup
		part	:HandGlowPsionics.part
		part	:HandGlowPsionicsdown.part
		Lifespan 20

	End

	Event
		ENAME	HandGlow
		Type	local
		At	hookup
		Part	:HandStreakPsionicsDown.part
		Part	:HandStreakPsionics.part
		Lifespan 20

	End

End

Condition
	On	Time
	Time	5


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther 	Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther 	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End


	Event
		ENAME	RightLarmGlow
		Type	local
		At	WepR
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther 	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 70

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther 	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	LeftLarmGlow
		Type	local
		At	WepL
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther Ulegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther Llegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		part	:HeadCirclesPsionics.part
		POther LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End


End

Condition
	On	Cycle
	Time	20
	Chance	.8


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:HeadCircles01.part
		part	:HeadCirclesDark.part
		part	:HeadCirclesPsionics.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 10

	End


End


End