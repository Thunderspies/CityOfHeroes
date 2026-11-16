#############################################################
## RockChunkBrownScatter.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	rocky
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			StartColor		255 255 255
			Scale			0.175 0.175 0.175
			InitialVelocityJitter	0.2 0.25 0.2
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PhysDebris		1
			PhysDensity		3
			FadeOutLength 		160
			TintGeom		1
		End
		Geom	Tintable_FX_Magma01
		Geom	Tintable_FX_Magma02
		Geom	Tintable_FX_Magma03
		Geom	Tintable_FX_Magma04
		Geom	Tintable_FX_Magma05
	End
End

#############################################################

End