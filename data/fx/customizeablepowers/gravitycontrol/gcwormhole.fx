#############################################################
## GCWormHole.fx
#############################################################

FxInfo

Lifespan 120

#############################################################

Condition
	On	Time
	Time	25

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	CustomizeablePowers\GravityControl\GCBodyGlowSlow.part
		Pmagnet Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 90
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	CustomizeablePowers\GravityControl\GCBodyGlowSlow.part
		Pmagnet Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 90
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	CustomizeablePowers\GravityControl\GCBodyGlowSlow.part
		Pmagnet LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 90
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	CustomizeablePowers\GravityControl\GCBodyGlowSlow.part
		Pmagnet LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 90
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	CustomizeablePowers\GravityControl\GCBodyGlowSlow.part
		Pmagnet LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 90
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	CustomizeablePowers\GravityControl\GCBodyGlowSlow.part
		Pmagnet LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 90
	End
End

#########################################################
################## BODY GLOW MIST #######################
#########################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet Neck
		Sound gravityzap5 100 100 0.85
		Lifespan 0
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet Chest
		Lifespan 0
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet LarmR
		Lifespan 0
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet LarmRnode
		Lifespan 0
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\GravityControl\GCBodyGlowMist.part
		Pmagnet LarmLNode
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

#############################################################

End