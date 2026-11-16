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
		Type	create
		Inherit	Position Rotation
		At	origin
	
		BhvrOverride
			PositionOffset		0 0 0
		End
	End

	
########  Object   #################

	Event
		PhysicsOnly
		Ename	Object
		Type	Create
		Inherit	All
		At	Base

		Geom	Univ_Book01
		Geom	Univ_Book02
		Geom	Univ_Book03
		Geom	Univ_Book04
		Geom	Univ_Book05
		Geom	Univ_Book06
		Geom	Univ_Book07
		Geom	Univ_Book08
		Geom	Univ_Book09
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			physJointCollidesWorld

			PositionOffset		0 0 0
			Scale			1.25 1.25 1.25
			
			pyrRotateJitter		0 0 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.2
			PhysSFriction		.4
			PhysDFriction		.5
			PhysScale		1.25 2 1.25
			PhysDensity		2

			#PhysDebris		3

			Drag			0.005
			PhysJointDrag		0.02
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX RotateY RotateZ TranslateX TranslateY TranslateZ
			PhysJointAngLimit	10
			PhysJointLinLimit	10
			PhysJointAngSpring	25
			PhysJointAngSpringDamp	5
			PhysJointBreakForce	1
			PhysJointBreakTorque	1
		End

		#JEvent	:IronRodGateJointBreak.fx
	End


End

End