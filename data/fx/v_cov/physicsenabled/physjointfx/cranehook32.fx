##############################
###  PHYS-FORCE Template   ###
##############################

FxInfo

#LifeSpan	300

########  Main   #################

Condition
	On	Time
	Time	0
	
	Event
		Ename	RopeHookup
		Type	create
		Inherit	Position Rotation Scale
		At	root  #origin
		BhvrOverride
			PositionOffset		0 0 0
		End
	End
	
########  swining Skeleton   #################

	
	Event
		Ename	SwiningSkeleton
		Type	Create
		Inherit	Position Rotation Scale
		At	RopeHookup

		Geom	_DC_crane_arm_cbl_32ft

		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			physJointCollidesWorld
			Physics			1
			PhysGravity		1.5
			PhysRestitution		.05
			PhysSFriction		.9
			PhysDFriction		.9

			Drag			0.025
			PhysJointDrag		0.025
			PhysJointAngSpringDamp	20
			PhysJointDOFs		RotateX RotateY RotateZ


			PhysJointAngLimit	75
			PhysJointAngSpring	100
			PhysDebris		3			
			PhysJointAnchor		0 -5 0

			PhysDensity		2
		End

	End
		
End
	
End