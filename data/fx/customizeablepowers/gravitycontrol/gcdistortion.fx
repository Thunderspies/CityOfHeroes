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
		Sound gravitylift 80 80 .88
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
		ENAME	LarmRNode
		Type	Local
		At	LarmR

	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	CustomizeablePowers\GravityControl\GCBodyGlow.part
		Pmagnet LarmRNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	CustomizeablePowers\GravityControl\GCBodyGlow.part
		Pmagnet LarmRnode
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
		ENAME	LarmRNode
		Type	Local
		At	LarmR

	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet LarmRNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet LarmRnode
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
	Time	20

	Event
		ENAME	Vortex
		Type	Posit
		At	WepL
		part	CustomizeablePowers\GravityControl\GCLiftingElement1out.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 580
	End

End

Condition
	On	Time
	Time	20

	Event
		ENAME	Vortex
		Type	Posit
		At	WepR
		part	CustomizeablePowers\GravityControl\GCLiftingElement1out.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 580
	End

End

End