#############################################################
## Child_RockScatter1.fx
#############################################################

FXInfo

Lifespan 400

#############################################################

Condition
	On 	Time
	Time 	3
	Repeat	7

	Event
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0 1 0
			StartJitter		3 2 3
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			TintGeom		1
		End
		Geom	Magma_FXDebris01A
		Geom	Magma_FXDebris02A
		Geom	Magma_FXDebris03A
		Geom	Magma_FXDebris04A
		Geom	Magma_FXDebris05A
		Lifespan 300
		LifespanJitter	90
		CThresh 	0.00001
	End
End

Condition
	On 	Time
	Time 	3
	Repeat	7

	Event
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0 1 0
			StartJitter		3 2 3
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			TintGeom		1
		End
		Geom	Lava_FXDebris01A
		Geom	Lava_FXDebris02A
		Geom	Lava_FXDebris03A
		Geom	Lava_FXDebris04A
		Geom	Lava_FXDebris05A
		Lifespan 300
		LifespanJitter	90
		CThresh 	0.00001
	End
End

#############################################################

End