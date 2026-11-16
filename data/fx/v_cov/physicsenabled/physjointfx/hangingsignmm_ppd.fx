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
		Type	Local
		At	origin
	End

	Event
		Ename	sign2
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	_MSign_PPD_Sign_Swing
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
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