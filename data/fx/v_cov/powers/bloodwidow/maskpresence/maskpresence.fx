#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		bhvr	behaviors\soundFade3.bhvr
		Sound NW_Mask_Presence_01 70 70 1
		Flags	PowerLoopingSound
		Lifespan 200
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	Chest
		Lifespan 30
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	LarmR
		Lifespan 30
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	LarmR
		Lifespan 30
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	LarmL
		Lifespan 30
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	LarmL
		Lifespan 30
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	Ulegl
		Lifespan 30
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	Llegl
		Lifespan 30
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	UlegR
		Lifespan 30
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark.part
		POther	LlegR
		Lifespan 30
	End
End

#############################################################

Condition
	On	Cycle
	Time	30

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	Neck
		Lifespan 30
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	LarmR
		Lifespan 30
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	LarmR
		Lifespan 30
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	LarmL
		Lifespan 30
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	LarmL
		Lifespan 30
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	Ulegl
		Lifespan 30
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	Llegl
		Lifespan 30
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	UlegR
		Lifespan 30
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	V_COV\Powers\Ninjitsu\HideLinesDark2.part
		part	V_COV\Powers\Ninjitsu\HideEnergyDark2.part
		POther	LlegR
		Lifespan 30
	End
End

#############################################################

End