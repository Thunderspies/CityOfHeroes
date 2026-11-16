#############################################################
## SweepingRay_Attack.fx
#############################################################

FxInfo
Flags DontSuppress

lifespan 110

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Anchor
		Type	Local
		At	T_Head
		BhvrOverride
			Scale 130.0 130.0 130.0
		End
		Geom	FX_ConeOFfsets
	End

	Event
		EName	AnchorPiv
		Type	Start
		At	Anchor
		AltPiv	1
	End
End

Condition
	On	Time
	Time	55

	Event
		Type	StartPositOnly
		At	AnchorPiv
		Part	:Projectile_Trail_Cone.part
		Lifespan	80
		POther	T_Head
		LookAt	T_Head
	End

	Event
		Type	StartPositOnly
		At	Anchor
		Part	:Projectile_Trail_Cone_Highlights.part
		Part	:Projectile_Trail_Core.part
		Part	:Projectile_Trail_Core_Alpha.part
		Part	:Projectile_Trail_Glow.part
		Part	:Base_Splash.part
		Part	:Base_Splash_Core.part
		Lifespan	80
		POther	AnchorPiv
		LookAt	AnchorPiv
	End

End



#############################################################

End