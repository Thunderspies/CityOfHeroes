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
		At	origin
	
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

########  Rope   #################

	Event
		Ename	velvetRope
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	_bnk_rope
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.01
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End


End


End