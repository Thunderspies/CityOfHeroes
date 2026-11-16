#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:PsionicBodyGlows01.part
		POther	Neck
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:PsionicBodyGlows01.part
		POther	Chest
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:PsionicBodyGlows01.part
		POther	LarmR
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:PsionicBodyGlows01.part
		POther	LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:PsionicBodyGlows01.part
		POther	LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:PsionicBodyGlows01.part
		POther	LarmL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:PsionicBodyGlows01.part
		POther	Ulegl
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:PsionicBodyGlows01.part
		POther	Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:PsionicBodyGlows01.part
		POther	UlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:PsionicBodyGlows01.part
		POther	LlegR
	End
End

#############################################################

Condition
	On	Cycle
	Time	40
	Chance	0.5

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:PsionicBodyGlowLines02.part
		POther	Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Sound	fast7 80 60 .6
		Lifespan	20
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:PsionicBodyGlowLines02.part
		POther	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:PsionicBodyGlowLines02.part
		POther	LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:PsionicBodyGlowLines02.part
		POther	LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:PsionicBodyGlowLines02.part
		POther	LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:PsionicBodyGlowLines02.part
		POther	LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:PsionicBodyGlowLines02.part
		POther	Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:PsionicBodyGlowLines02.part
		POther	Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:PsionicBodyGlowLines02.part
		POther	UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:PsionicBodyGlowLines02.part
		POther	LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	20
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	Neck
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		#part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	Chest
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	V_COV\Powers\Ninjitsu\BodyStreaks01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	V_COV\Powers\Ninjitsu\BodyStreaks01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	LarmL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	Ulegl
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	V_COV\Powers\Ninjitsu\BodyStreaks01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part1	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	UlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	V_COV\Powers\Ninjitsu\BodyStreaks01.part
		part	V_COV\Powers\Ninjitsu\BodyGlowLines01.part
		part	V_COV\Powers\Ninjitsu\BodyGlows01.part
		Part	V_COV\Powers\Ninjitsu\BodyWavesDark.part
		POther	LlegR
	End
End

#############################################################

End