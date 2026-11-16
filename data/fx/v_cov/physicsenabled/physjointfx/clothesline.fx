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
		Type	start
		At	root  #origin
	
		BhvrOverride
			PositionOffset		0 0 0
		End
	End
	

########  Line   #################

	Event
		Ename	clothesline
		Type	Create
		Inherit	Position Rotation
		At	Base

		Geom	_clothesline_32
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			Alpha			1
		End
	End

End

#########  clothes   #################

Condition
	On	Time
	Time	0
	Chance	.8

	Event
		Ename	clothes01
		Type	Create
		Inherit	Position Rotation
		At	clothesline
		AltPiv	1

		Geom	_Clothesline_item_01
		Geom	_Clothesline_item_02
		Geom	_Clothesline_item_03
		Geom	_Clothesline_item_04
		Geom	_Clothesline_item_05
		Geom	_Clothesline_item_06
		Geom	_Clothesline_item_07
		Geom	_Clothesline_item_08
		Geom	_Clothesline_item_09
		Geom	_Clothesline_item_10
		Geom	_Clothesline_item_11
		Geom	_Clothesline_item_12
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			PhysScale		.5 .5 2

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.6
			PhysDFriction		.7

			Drag			0.05
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End
End
Condition
	On	Time
	Time	0
	Chance	.8

	Event
		Ename	clothes01
		Type	Create
		Inherit	Position Rotation
		At	clothesline
		AltPiv	2

		Geom	_Clothesline_item_01
		Geom	_Clothesline_item_02
		Geom	_Clothesline_item_03
		Geom	_Clothesline_item_04
		Geom	_Clothesline_item_05
		Geom	_Clothesline_item_06
		Geom	_Clothesline_item_07
		Geom	_Clothesline_item_08
		Geom	_Clothesline_item_09
		Geom	_Clothesline_item_10
		Geom	_Clothesline_item_11
		Geom	_Clothesline_item_12
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			PhysScale		.5 .5 2

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.6
			PhysDFriction		.7

			Drag			0.05
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End
End
Condition
	On	Time
	Time	0
	Chance	.8

	Event
		Ename	clothes01
		Type	Create
		Inherit	Position Rotation
		At	clothesline
		AltPiv	3

		Geom	_Clothesline_item_01
		Geom	_Clothesline_item_02
		Geom	_Clothesline_item_03
		Geom	_Clothesline_item_04
		Geom	_Clothesline_item_05
		Geom	_Clothesline_item_06
		Geom	_Clothesline_item_07
		Geom	_Clothesline_item_08
		Geom	_Clothesline_item_09
		Geom	_Clothesline_item_10
		Geom	_Clothesline_item_11
		Geom	_Clothesline_item_12
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			PhysScale		.5 .5 2

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.6
			PhysDFriction		.7

			Drag			0.05
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End
End
Condition
	On	Time
	Time	0
	Chance	.8

	Event
		Ename	clothes01
		Type	Create
		Inherit	Position Rotation
		At	clothesline
		AltPiv	4

		Geom	_Clothesline_item_01
		Geom	_Clothesline_item_02
		Geom	_Clothesline_item_03
		Geom	_Clothesline_item_04
		Geom	_Clothesline_item_05
		Geom	_Clothesline_item_06
		Geom	_Clothesline_item_07
		Geom	_Clothesline_item_08
		Geom	_Clothesline_item_09
		Geom	_Clothesline_item_10
		Geom	_Clothesline_item_11
		Geom	_Clothesline_item_12
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			PhysScale		.5 .5 2

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.6
			PhysDFriction		.7

			Drag			0.05
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End
End
Condition
	On	Time
	Time	0
	Chance	.8

	Event
		Ename	clothes01
		Type	Create
		Inherit	Position Rotation
		At	clothesline
		AltPiv	5

		Geom	_Clothesline_item_01
		Geom	_Clothesline_item_02
		Geom	_Clothesline_item_03
		Geom	_Clothesline_item_04
		Geom	_Clothesline_item_05
		Geom	_Clothesline_item_06
		Geom	_Clothesline_item_07
		Geom	_Clothesline_item_08
		Geom	_Clothesline_item_09
		Geom	_Clothesline_item_10
		Geom	_Clothesline_item_11
		Geom	_Clothesline_item_12
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			PhysScale		.5 .5 2

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.6
			PhysDFriction		.7

			Drag			0.05
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End
End
Condition
	On	Time
	Time	0
	Chance	.8

	Event
		Ename	clothes01
		Type	Create
		Inherit	Position Rotation
		At	clothesline
		AltPiv	6

		Geom	_Clothesline_item_01
		Geom	_Clothesline_item_02
		Geom	_Clothesline_item_03
		Geom	_Clothesline_item_04
		Geom	_Clothesline_item_05
		Geom	_Clothesline_item_06
		Geom	_Clothesline_item_07
		Geom	_Clothesline_item_08
		Geom	_Clothesline_item_09
		Geom	_Clothesline_item_10
		Geom	_Clothesline_item_11
		Geom	_Clothesline_item_12
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			PhysScale		.5 .5 2

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.6
			PhysDFriction		.7

			Drag			0.05
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End
End
Condition
	On	Time
	Time	0
	Chance	.8

	Event
		Ename	clothes01
		Type	Create
		Inherit	Position Rotation
		At	clothesline
		AltPiv	7

		Geom	_Clothesline_item_01
		Geom	_Clothesline_item_02
		Geom	_Clothesline_item_03
		Geom	_Clothesline_item_04
		Geom	_Clothesline_item_05
		Geom	_Clothesline_item_06
		Geom	_Clothesline_item_07
		Geom	_Clothesline_item_08
		Geom	_Clothesline_item_09
		Geom	_Clothesline_item_10
		Geom	_Clothesline_item_11
		Geom	_Clothesline_item_12
		
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			PhysScale		.5 .5 2

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.6
			PhysDFriction		.7

			Drag			0.05
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX
		End
	End
End

End