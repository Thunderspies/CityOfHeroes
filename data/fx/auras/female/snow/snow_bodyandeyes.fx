#############################################################
## SnowBody.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:ColdMist.part
		part	:ColdMist_Additive.part
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:ColdMist.part
		part	:ColdMist_Additive.part
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:ColdMist_Additive.part
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:ColdMist_Additive.part
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Chest
		part	:ColdMist_Trail.part
		part	:ColdMist_Highlight_Trail.part
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:ColdMist.part
		part	:ColdMist_Additive.part
		part	:ColdMist_Trail.part
		part	:ColdMist_Highlight_Trail.part
	End

	Event
		ENAME	LeftLLegGlow
		Type	PositOnly
		At	LLegL
		part	:ColdMist.part
		part	:ColdMist_Additive_Stationary.part

	End

	Event
		ENAME	LeftLLegGlow
		Type	PositOnly
		At	LLegR
		part	:ColdMist.part
		part	:ColdMist_Additive_Stationary.part

	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:Eye_GlowL.part
		Part	:Eye_Glow.part
		Part	:SnowFlakes_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:Eye_GlowR.part
		Part	:Eye_Glow.part
		Part	:SnowFlakes_Eyes.part
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:Snowflakes.part
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:Snowflakes.part
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Snowflakes.part
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:Snowflakes.part
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:Snowflakes.part
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:Snowflakes.part
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Snowflakes.part
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	LLegL
		part	:Snowflakes.part
	End

End