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
		Ename	Base
		Type	Local
		At	root  #origin
	
		BhvrOverride
			PositionOffset		0 0 0
		End
	End
	

	
########  Sign  #################

	Event
		Ename	sign2
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	Univ_Sign
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.02

			#PhysDebris		2
			PhysDensity		2
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateZ
			PhysJointAngLimit	90
			
			#PhysJointBreakForce	8000
		End
	End

End


End