#########################################################
##	template

FxInfo

LifeSpan	500

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Ename	RopeHookup
		Type	create
		Inherit	Position Rotation Scale
		At	origin
	
		Geom	FishWinchHookup
		
	End

######################## Hanging Rope ##########################

	Event
		Ename	SwiningShark
		Type	Create
		Inherit	Position Rotation Scale
		At	RopeHookup

		Geom	FishWinch

		Bhvr	Behaviors/FadeOut.bhvr
		
		BhvrOverride

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		1 1 1

			Drag			0.025
			PhysJointDrag		0.075
			
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	180
			PhysJointAngSpringDamp	50
			PhysDensity		2
		End

		LifeSpan	600
	End

###############	Shark break event	#########################################
	
	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 1
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.08
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.15
			physSFriction 	 	0.15
			physDFriction 	 	0.15
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_Swordfish

		Lifespan	1
	End


End	

End	

	