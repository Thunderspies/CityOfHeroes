#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ConeOffset
		Type	Start
		At	Root
		BhvrOverride
			PYRRotateJitter	0 0 60
			Scale		20 20 20
		End
		GEOM	FX_ConeOffsets
	End

	Event
		EName 	TargetOffset
		Type	Local
		At	ConeOffset
		AltPiv	1
	End

	Event
		EName 	ArcOffsets
		Type	Start
		At	C_Hair
		Bhvr	ENEMYFX\Clockwork\Antimatter\LightningArc1.bhvr
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
	Time 	25

	Event
		Type	Local
		At	Head #C_Hair
		Part	:LightningSpray_Small.part
		Part	:LightningSpray_RetinaBurn.part
		POther 	ArcOffset1
	End

	Event
		Type	Local
		At	ArcOffset1
		Part	:LightningSpray_Small.part
		Part	:LightningSpray_RetinaBurn.part
		POther 	ArcOffset2
	End
End

Condition
	On	Time
	Time	26

	Event
		Type	Local
		At	ArcOffset2
		Part	:LightningSpray_Small.part
		Part	:LightningSpray_RetinaBurn.part
		POther	ArcOffset3
	End

	Event
		Type	Local
		At	ArcOffset3
		Part	:LightningSpray_Small.part
		Part	:LightningSpray_RetinaBurn.part
		POther 	ArcOffset4
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Local
		At	ArcOffset5
		Part	:LightningSpray_Small.part
		Part	:LightningSpray_RetinaBurn.part
		POther 	TargetOffset
	End
End

#############################################################

End