#############################################################
## RockChunkBrownScatter.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			Scale			0.2 0.2 0.2
			InitialVelocity		0.0 0.1 0.0
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
		Geom	CrystalShard01
		Geom	CrystalShard02
	End
End

#############################################################

End