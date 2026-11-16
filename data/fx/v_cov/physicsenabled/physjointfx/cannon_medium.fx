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
		Ename	CannonHookup
		Type	create
		Inherit	Position Rotation Scale
		At	origin
	
		BhvrOverride
			PositionOffset	0 0 0
		End
		
	End
	
########  swining Skeleton   #################

	
	Event
		Ename	cannon
		Type	Create
		Inherit	Position Rotation Scale
		At	CannonHookup

		Geom	CannonBarrel_Med

		
		BhvrOverride
#			physJointCollidesWorld
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.025
			PhysJointDrag		0.05
			PhysJointAngSpringDamp	120
			PhysJointDOFs		RotateX TranslateX 
			physJointLinSpring	9999999
			physJointLinSpringDamp	999

			PhysJointAngLimit	20
			PhysJointAngSpring	500
#			PhysDebris		2			
#			PhysJointAnchor		-.5 0 0

#			physJointBreakTorque	99999
#			PhysJointBreakForce	1000
			PhysDensity		2
		End

	End
		
End
	
End