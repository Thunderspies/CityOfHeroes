#############################################################
## IllusBlind.fx
#############################################################

FxInfo
Lifespan 200

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound blind 80 80 .8
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	EyeNode
		Type	Local
		At	Hair
		Geom	HairToEyesAdjustment
	End
End

Condition
	On	Time
	Time	14

	Event
		ENAME	EyeGlow
		Type	Local
		At	EyeNode
		AltPiv	1
		part	CustomizeablePowers\IllusionControl\IllusBlindOrbits01.part
		POther Neck
		Lifespan 10
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	EyeGlow
		Type	Local
		At	EyeNode
		AltPiv	1
		part	CustomizeablePowers\IllusionControl\IllusBlindGlow02.part
		part	CustomizeablePowers\IllusionControl\IllusBlindOrbits01.part
		POther Neck
		Lifespan 10
	End
End

#############################################################

End