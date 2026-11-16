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
			Scale			0.875 0.875 0.875
			InitialVelocityJitter	0.2 0.25 0.2
			StartColor		255 255 255
			PrimaryTint		66
			SecondaryTint		33
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PhysDebris		1
			PhysDensity		3
			FadeOutLength 		160
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