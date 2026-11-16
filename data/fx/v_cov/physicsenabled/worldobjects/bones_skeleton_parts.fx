#########################################################
##	template

FxInfo


#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Ename	RopeHookup
		Type	create
		Inherit	Position Rotation Scale
		At	origin
	
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

###############	Skeleton Parts	#########################################
	
	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_Skull

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonRibCage

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonPelvis_Spine

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonUpperArm

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonLowerArm_Hand

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonUpperArm2

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonLowerArm

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonHand

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonUpperLeg

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonLowerLegFoot

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonUpperLegRight

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonLowerLegRight

	End

	Event
		Type	create
		Inherit	Position Rotation Scale
		At	RopeHookup
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		3 0 3
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
		End

		CThresh 	0.000001
		
		Geom		FX_SkeletonFoot

	End
	
	
	
	
	
	


End	

End	

	