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
		At	origin
	
		Geom	RopeHookup
	End
	
########  swining Skeleton   #################

	
	Event
		Ename	SwiningSkeleton
		Type	Create
		Inherit	Position Rotation Scale
		At	RopeHookup

		Geom	_doc_swdf_01

		
		BhvrOverride
			PyrRotate	0 -95 0
			physJointCollidesWorld
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.025
			PhysJointDrag		0.05
			PhysJointAngSpringDamp	20
			PhysJointDOFs		RotateX RotateY RotateZ


#			PhysJointAngLimit	180
			PhysJointAngSpring	500
#			PhysDebris		2			
#			PhysJointAnchor		-.5 0 0

#			physJointBreakTorque	9999
			PhysJointBreakForce	 200
			PhysDensity		2
		End

		JEvent	:SwordfishJointBreak.fx
		JDestroy	1
	End
		
End
	
End