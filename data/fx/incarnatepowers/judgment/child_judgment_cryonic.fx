#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	IceAnchor
		Type	Local
		At	WepL
		BhvrOverride
			PyrRotate	-50 -75 0
			PyrRotateJitter	15 15 0
			Scale		50 50 50
		End
		Geom	FX_ConeOffsets

		Lifespan 30
	End

	Event
		Ename	IceAnchor2
		Type	Start
		At	WepL
		BhvrOverride
			PyrRotate	-50 -75 0
			PyrRotateJitter	8 8 0
			Scale		50 50 50
		End
		Geom	FX_ConeOffsets

		Lifespan 30

	End

	Event
		Ename	TargetOffset2
		Type	Start
		At	IceAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset	0 10 0
			PyrRotate	180 0 0
		End
		Lifespan 30
	End

	Event
		Ename	TargetOffset3
		Type	Start
		At	IceAnchor2
		AltPiv	2
		BhvrOverride
			PositionOffset	0 0 0
			PyrRotate	180 0 0
		End
		Lifespan 30

	End

	Event
		Ename	Prime
		Type	Start
		At	WepL
		Bhvr	:Icicles_Projectiles.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			InitialVelocity		0.0 0.0 0.0
			Scale			0.4 0.4 0.8
			StartJitter 		0.1 0.1 0.1
			TrackRate		2.0
			TintGeom		1
		End
		Geom	tintable_FX_IceBlast2
		Part	:Projectile_Trail.part
		LookAt	TargetOffset2
		Magnet	TargetOffset2
		Lifespan 15
	End

	Event
		Ename	Prime2
		Type	Start
		At	WepL
		Bhvr	:Icicles_Projectiles_Large.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			InitialVelocity		0.0 0.0 0.0
			Scale			0.8 0.8 2
			StartJitter 		0.1 0.1 0.1
			TrackRate		3.0
			TintGeom		1
		End
		Geom	tintable_FX_IceBlast2
		Part	:Projectile_Trail.part
		LookAt	TargetOffset3
		Magnet	TargetOffset3
		Lifespan 13
	End
End

#############################################################

End