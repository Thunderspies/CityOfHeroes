##############################
###  PHYS-FORCE Template   ###
##############################

FxInfo

########  Physics Hanging Sign   #################

Condition
	On	Time
	Time	0
	
	Event
		Ename	Base
		Type	Local
		At	origin
	End
	
	Event
		Ename	sign
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	_sign_hnging_RuinedDirtyDuckBar
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		-0.812988 -9.3 0
			Scale			1 1 1
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			Drag			0.01
			PhysDebris		2
			PhysJointAnchor		0 5 0
			PhysJointDOFs		RotateX
			PhysJointAngLimit	170
		End
	End
End


End