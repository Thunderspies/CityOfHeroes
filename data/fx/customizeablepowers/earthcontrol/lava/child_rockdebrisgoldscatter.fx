#############################################################
## RockDebrisGoldScatter.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			Scale			0.75 0.75 0.75
			InitialVelocityJitter	0.2 0.25 0.2
			StartColor		255 240 192
			PrimaryTint		100
			SecondaryTint		0
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			PhysDebris		1
			PhysDensity		3
			FadeOutLength 		160
			TintGeom		1
		End
		Geom	Lava_FXDebris01A
		Geom	Lava_FXDebris02A
		Geom	Lava_FXDebris03A
		Geom	Lava_FXDebris04A
		Geom	Lava_FXDebris05A
	End
End

#############################################################

End