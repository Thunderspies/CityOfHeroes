#############################################################
## Small_RockChunkGrayScatter.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0
	Random	1

	Event
		EName 	rocky
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			Scale			0.75 0.75 0.75
			InitialVelocityJitter	0.2 0.25 0.2
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PhysDebris		3
			PhysDensity		2
			FadeOutLength 		160
			TintGeom		1
		End
		Geom	Earth_FXDebris01A
		Geom	Earth_FXDebris02A
		Geom	Earth_FXDebris03A
		Geom	Earth_FXDebris04A
		Geom	Earth_FXDebris05A
	End
End

#############################################################

End