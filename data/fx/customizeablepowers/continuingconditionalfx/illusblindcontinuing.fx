#############################################################
## IllusBlindContinuing.fx
#############################################################

FxInfo
ClampMaxScale 2.0 2.0 2.0

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound forcefield2_loop 80 80 0.22
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

	Event
		ENAME	ChestGlow
		Type	Local
		At	EyeNode
		AltPiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:IllusBlindGlow01.part
		part	:IllusBlindOrbits01.part
		POther	Neck
	End
End

#############################################################

End