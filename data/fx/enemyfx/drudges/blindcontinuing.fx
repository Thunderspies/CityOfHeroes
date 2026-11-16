#############################################################
## Blind - Continuing FX
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
		At	Root
		Sound 	Incarnate_Reactive_Loop 100 100 .3
		bhvr	behaviors/Sound/SoundIn05Out10.bhvr
		lifespan 45
	End

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
		part	:BlindGlow01.part
		part	:BlindOrbits01.part
		POther	Neck
	End

End

#############################################################

End