#############################################################
## Blind - Continuing FX
#############################################################

FxInfo

ClampMaxScale 2.0 2.0 2.0

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound forcefield2_loop 80 80 .22
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
		part	CustomizeablePowers\IllusionControl\IllusBlindGlow01.part
		part	CustomizeablePowers\IllusionControl\IllusBlindOrbits01.part
		POther	Neck
	End
End

#############################################################

End