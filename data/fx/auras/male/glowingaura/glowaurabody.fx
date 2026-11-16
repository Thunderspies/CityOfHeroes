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
		part	:BodyAura01.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
		part	:BodyAura01.part
		Pmagnet Head
	End

	Event
		Type	Local
		At	UarmR
		part	:BodyAura01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:BodyAura01.part
		Pmagnet UarmL
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyAura01.part
		Pmagnet Neck
		POther  Neck
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyAura01.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyAura01.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
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
		part	:BodyAura01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:BodyAura01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:BodyAura01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
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
		part	:LegAura01.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:LegAura01.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		part	:BodyAura01.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	:BodyAura01.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	FootR
		part	:BodyAura01.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	:BodyAura01.part
		Pmagnet FootL
	End
End

#############################################################

End