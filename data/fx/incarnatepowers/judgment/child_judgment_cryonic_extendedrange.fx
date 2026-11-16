#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
Lifespan 90

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
		At	Root
		BhvrOverride
			PyrRotateJitter	0 0 180
			Scale		65 65 65
		End
		Geom	FX_ConeOffsets

		Lifespan 70

	End

	Event
		Ename	TargetOffset2
		Type	Start
		At	IceAnchor
		AltPiv	2
		BhvrOverride
			PyrRotate	180 0 0

		End
	#	Geom	GEO_FX_TestSphere
		Lifespan 70

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
		End
		Geom	tintable_FX_IceBlast
		Part	:Projectile_Trail.part
	#	Geom	Tintable_Icicle02
		#Geom	Tintable_Icicle03
		LookAt	TargetOffset2
		Magnet	TargetOffset2
		Lifespan 50
	End
End

#############################################################

End