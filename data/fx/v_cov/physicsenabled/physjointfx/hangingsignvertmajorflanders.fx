##############################
###  PHYS-FORCE Template   ###
##############################

FxInfo

########  Main   #################

Condition
	On	Time
	Time	0
	
	Event
		Ename	Base
		Type	start
		At	root  #origin
	End
	
	Event
		Ename	sign1
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	_sign_hnging_vert_MajorFlanders
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		-0.749023 -16.23457 0
			Scale			1 1 1
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			Drag			0.01
			PhysJointDrag		0.015
			PhysDebris		2
			PhysJointAnchor		0 11.5 0
			PhysJointDOFs		RotateX
			PhysJointAngLimit	170
		End
	End
End


End