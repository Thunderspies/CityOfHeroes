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
	

	
########  Sign  #################

	Event
		Ename	chair
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	_LB_Tek_Chair_Top
		
		BhvrOverride
			PositionOffset		0 0 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3

			Drag			0.5
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateY
		End
	End

End


End