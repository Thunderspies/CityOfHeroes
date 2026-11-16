#############################################################
## RockDebrisGoldExplode.fx
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
			StartColor		255 255 255
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	1 0.75 1
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
		Geom	Crystal_FXDebris01A
		Geom	Crystal_FXDebris02A
		Geom	Crystal_FXDebris03A
		Geom	Crystal_FXDebris04A
		Geom	Crystal_FXDebris05A
	End
End

#############################################################

End