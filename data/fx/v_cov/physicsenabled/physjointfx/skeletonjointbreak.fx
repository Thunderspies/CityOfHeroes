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
	
		Geom	RopeHookup

	End

######################## Hanging Rope ##########################

	Event
		Ename	SwiningSkeleton
		Type	Create
		Inherit	Position Rotation Scale
		At	RopeHookup

		Geom	FX_Gallows_Rope

		Bhvr	Behaviors/FadeOut.bhvr

		BhvrOverride


			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			PhysScale		1 1 2

			Drag			0.025
			PhysJointDrag		0.075
			
			PhysJointDOFs		RotateX RotateY RotateZ
			PhysJointAngLimit	180
			PhysJointAngSpringDamp	50
			PhysDensity		2
		End

		LifeSpan	600
	End

###############	Skeleton Parts	#########################################
	
	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 1
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_Skull

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 2
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonRibCage

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 3
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonPelvis_Spine

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 4
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonUpperArm

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 5
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonLowerArm_Hand

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 6
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonUpperArm2

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 7
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonLowerArm

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 8
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonHand

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 9
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonUpperLeg

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 10
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonLowerLegFoot

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 11
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonUpperLegRight

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 12
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonLowerLegRight

		Lifespan	1
	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Altpiv 13
		BhvrOverride
			InitialVelocityJitter	.2 .2 .2
			SpinJitter		.5 .5 .5
			Gravity			.048		
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
#			Scale			.15 .15 .15
			Physdebris	3
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonFoot

		Lifespan	1
	End
	
	
	
	
	
	


End	

End	

	