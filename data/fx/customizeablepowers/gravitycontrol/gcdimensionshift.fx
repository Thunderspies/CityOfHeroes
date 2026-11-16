#########################################################
##	template

FxInfo
Lifespan 60



#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
		Sound gravityzap2 80 80 .75
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	NeckNode
		Type	Local
		At	Neck

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	CustomizeablePowers\GravityControl\GCBodyGlow.part
		Pmagnet NeckNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	ChestNode
		Type	Local
		At	Chest

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	CustomizeablePowers\GravityControl\GCBodyGlow.part
		Pmagnet ChestNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	CustomizeablePowers\GravityControl\GCBodyGlow.part
		Pmagnet LarmLNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	CustomizeablePowers\GravityControl\GCBodyGlow.part
		Pmagnet LarmLNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

End

#########################################################
################## BODY GLOW MIST #######################
#########################################################



Condition
	On	Time
	Time	0

	Event
		ENAME	NeckNode
		Type	Local
		At	Neck

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet NeckNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	ChestNode
		Type	Local
		At	Chest

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet ChestNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet LarmLNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet LarmLNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

End

#########################################################
#################### HAND ORBITS ########################
#########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	HandLOrbit
		Type	Local
		At	WepL
		part1	CustomizeablePowers\GravityControl\GCHandOrbits.part
		Pmagnet WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 40

	End

End

End