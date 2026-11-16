#############################################################
## GlowAuraBody.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Bhvr	behaviors/GenericParticleFade.bhvr

		part	:BodyAura01.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.1 0.1
		End
		part	:BodyAura01.part
		Pmagnet Head
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet UarmL
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet Neck
		POther  Neck
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet UarmR
		POther  UarmR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet LarmL
		POther  LarmL
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:LegAura01.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:LegAura01.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyAura01.part
		Pmagnet FootL
	End
End

#############################################################

End