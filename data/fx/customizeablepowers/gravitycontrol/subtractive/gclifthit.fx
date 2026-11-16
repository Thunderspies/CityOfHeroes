#############################################################
## GCLiftHit.fx
#############################################################

FxInfo
LifeSpan 200

#############################################################

Input
	Inpname	Hips
End
#############################################################

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	LevitationForce
		Type	PositOnly
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		8
			PhysForcePower		20
			PhysForcePowerJitter	5
		End

	End

	Event
		Ename	ScatterForce
		Type	PositOnly
		At	Hips

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		12
			PhysForcePower		4
			PhysForcePowerJitter	8
		End

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet Neck
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:GCBodyGlow.part
		Pmagnet Chest
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:GCBodyGlow.part
		Pmagnet LarmR
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:GCBodyGlow.part
		Pmagnet LarmR
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:GCBodyGlow.part
		Pmagnet LarmL
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlow.part
		Pmagnet LarmL
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part1	:GCBodyGlow.part
		Pmagnet Llegl
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:GCBodyGlow.part
		Pmagnet Llegl
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part1	:GCBodyGlow.part
		Pmagnet LlegR
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:GCBodyGlow.part
		Pmagnet LlegR
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0

	End


End

#########################################################
#################### BODY GLOW 2 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlowMist.part
		Pmagnet Neck
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:GCBodyGlowMist.part
		Pmagnet Chest
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:GCBodyGlow.part
		Pmagnet LarmR
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:GCBodyGlowMist.part
		Pmagnet LarmR
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmL
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmL
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part1	:GCBodyGlowMist.part
		Pmagnet Llegl
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:GCBodyGlowMist.part
		Pmagnet Llegl
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part1	:GCBodyGlowMist.part
		Pmagnet LlegR
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:GCBodyGlowMist.part
		Pmagnet LlegR
		Bhvr	CustomizeablePowers\GravityControl\GravityControlFade1.bhvr
		Lifespan 0
	End


End

#########################################################
####################   STREAKS   ########################
#########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	HandLStreaks
		Type	Posit
		At	HandL
		part1	:GCStreaks1.part
		Lifespan 60

	End

	Event
		ENAME	HandLStreaks
		Type	Posit
		At	HandR
		part1	:GCStreaks1.part
		Lifespan 60

	End

	Event
		ENAME	HandLStreaks
		Type	Posit
		At	FootR
		part1	:GCStreaks1.part
		Lifespan 60

	End

	Event
		ENAME	HandLStreaks
		Type	Posit
		At	FootL
		part1	:GCStreaks1.part
		Lifespan 60

	End

End

#########################################################
####################  GROUND FX  ########################
#########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	GroundSwirl
		Type	Start
		At	Root
		part	:GCVortexGround.part
		part	:GCVortexGlowsOut.part
		part	:GCFlashLrg.part
		Lifespan 30

	End


	Event
		Ename	LevitationForce
		Type	Start
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		125
			PhysForcePowerJitter	25
		End

		Lifespan	20

	End

End

End