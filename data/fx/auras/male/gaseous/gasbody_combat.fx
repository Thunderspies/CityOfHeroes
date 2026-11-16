#############################################################
## GasBody.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On 		Triggerbits
	Triggerbits 	COMBAT
	DoMany		1

	Event
		EName	UArmRAnchor
		Type	Local
		At	UarmR
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet UarmR
	End

	Event
		EName	FootRAnchor
		Type	Local
		At	FootR
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet FootR
	End

	Event
		EName	FootLAnchor
		Type	Local
		At	FootL
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet FootL
	End

	Event
		EName	UArmLAnchor
		Type	Local
		At	UarmL
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet UarmL
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet Neck
		POther  Neck
	End

#############################################################

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		part	:BodyGasTrail01.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet UarmR
		POther  UarmR
	End

#############################################################

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		EName	LLegRAnchor
		Type	Local
		At	LlegR
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		EName	LLegLAnchor
		Type	Local
		At	LlegL
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet FootL
		POther  FootL
	End
End

#############################################################

End