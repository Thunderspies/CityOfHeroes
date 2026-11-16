#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo

LifeSpan 3

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ConeOffset
		Type	Start
		At	Root
		BhvrOverride
			PYRRotateJitter	0 15 0
			Scale		20 20 20
		End
		GEOM	FX_ConeOffsets
	End

	Event
		EName 	TargetOffset
		Type	Local
		At	ConeOffset
		BhvrOverride
			StartJitter	0 0 0.625
		End
		AltPiv	1
	End

	Event
		EName 	ArcOffsets
		Type	Posit
		At	C_Hair
		Bhvr	ENEMYFX\Clockwork\Antimatter\LightningArc1.bhvr
		BhvrOverride
			Alpha	1
		End
		geom	FX_ElectricArc_Offsets_01
		Magnet	TargetOffset
		LookAt	TargetOffset
	End
End

###############################################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ArcOffset1
		Type	Local
		At	ArcOffsets
		AltPiv	1
		BhvrOverride
			StartJitter	0.1 0.1 0.1
		End
	End

	Event
		Ename	ArcOffset2
		Type	Local
		At	ArcOffsets
		AltPiv	2
		BhvrOverride
			StartJitter	0.1 0.1 0.1
		End
	End

	Event
		Ename	ArcOffset3
		Type	Local
		At	ArcOffsets
		AltPiv	3
		BhvrOverride
			StartJitter	0.1 0.1 0.1
		End
	End

	Event
		Ename	ArcOffset4
		Type	Local
		At	ArcOffsets
		AltPiv	4
		BhvrOverride
			StartJitter	0.1 0.1 0.1
		End
	End
End

###############################################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	C_Hair
		Part	:LightningSpray_Small.part
		POther 	ArcOffset1
	End

	Event
		Type	Local
		At	ArcOffset1
		Part	:LightningSpray_Small.part
		POther 	ArcOffset2
	End
End

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	ArcOffset2
		Part	:LightningSpray_Small.part
		POther	ArcOffset3
	End

	Event
		Type	Local
		At	ArcOffset3
		Part	:LightningSpray_Small.part
		POther 	ArcOffset4
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	ArcOffset4
		Part	:LightningSpray_Small.part
		POther	TargetOffset
	End
End

#############################################################

End