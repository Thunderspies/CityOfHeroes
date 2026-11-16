#############################################################
## GCDimensionShiftContinuing_WithFade.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	75

	Event
		Type	Local
		At 	origin
		Sound glow_loop 80 80 0.45
	End

	Event
		Type	Local
		At 	origin
		Sound glow6_loop 80 80 0.25
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Alpha
		Type	Local
		At	Origin
		Bhvr	:DimensionShift.bhvr
		Flags	AdoptParentEntity
	End

	Event
		ENAME	Vortex
		Type	Local
		At	Hips
		Part	:GCVortexSteamSmall.part
		LifeSpan 80
	End
End

Condition
	On	Time
	Time	5

	Event
		ENAME	Vortex
		Type	Local
		At	Hips
		part	:GCVortexSmall.part
		LifeSpan 60
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	Vortex
		Type	Local
		At	Hips
		Part	:GCVortexGlowsSmall.part
		Sound gravityhit 80 80 0.8
		Pmagnet Hips
		LifeSpan 80
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:GCBodyGlow.part
		Pmagnet Neck
		Lifespan 0
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Part	:GCBodyGlow.part
		Pmagnet Chest
		Lifespan 0
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Part	:GCBodyGlow.part
		Pmagnet LarmR
		Lifespan 0
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:GCBodyGlow.part
		Pmagnet LarmR
		Lifespan 0
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Part	:GCBodyGlow.part
		Pmagnet LarmL
		Lifespan 0
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:GCBodyGlow.part
		Pmagnet LarmL
		Lifespan 0
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Part	:GCBodyGlow.part
		Pmagnet Llegl
		Lifespan 0
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Part	:GCBodyGlow.part
		Pmagnet Llegl
		Lifespan 0
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Part	:GCBodyGlow.part
		Pmagnet LlegR
		Lifespan 0
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Part	:GCBodyGlow.part
		Pmagnet LlegR
		Lifespan 0
	End
End

#############################################################

End