#############################################################
## GasBody.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		part	:BodyGas01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	FootR
		part	:BodyGas01.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	:BodyGas01.part
		Pmagnet FootL
	End

	Event
		Type	Local
		At	UarmL
		part	:BodyGas01.part
		Pmagnet UarmL
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyGas01.part
		Pmagnet Neck
		POther  Neck
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyGas01.part
		part	:BodyGasTrail01.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyGas01.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:BodyGas01.part
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
		part	:BodyGas01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:BodyGas01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:BodyGas01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:BodyGas01.part
		Pmagnet LarmL
		POther  LarmL
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodyGas01.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodyGas01.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		part	:BodyGas01.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	:BodyGas01.part
		Pmagnet FootL
		POther  FootL
	End
End

#############################################################

End