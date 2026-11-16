#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Fast2 70 70 .7
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	5


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLinesDark.part
		part1	CustomizeablePowers\Ninjitsu\HeadCircles02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End


End

Condition
	On	Cycle
	Time	40
	Chance	.7

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	CustomizeablePowers\Ninjitsu\HeadCircles01.part
		part1	CustomizeablePowers\Ninjitsu\HeadCirclesDark.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 50

	End


End

Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther Neck
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther Chest
	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LarmR
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Ninjitsu\BodyStreaks01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LarmR
	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Ninjitsu\BodyStreaks01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LarmL
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther Ulegl
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Ninjitsu\BodyStreaks01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part1	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther UlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	CustomizeablePowers\Ninjitsu\BodyStreaks01.part
		part	CustomizeablePowers\Ninjitsu\BodyGlowLines01.part
		part	CustomizeablePowers\Ninjitsu\BodyGlows01.part
		Part	CustomizeablePowers\Ninjitsu\BodyWavesDark.part
		POther LlegR
	End


End


End