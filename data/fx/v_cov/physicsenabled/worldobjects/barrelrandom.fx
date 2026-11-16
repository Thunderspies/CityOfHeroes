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

		Geom	Chem_55gal_01
		
		Geom	JunkBarrel_55gal_01
		Geom	JunkBarrel_55gal_01

		Geom	JunkBarrel_55gal_02
		Geom	JunkBarrel_55gal_02
		Geom	JunkBarrel_55gal_02

		#Geom	OilDrum
		
		Bhvr	:GenericObjectFade.bhvr
		BhvrOverride
			physJointCollidesWorld

			PositionOffset		0 0 0
			Scale			1 1 1
			
			pyrRotateJitter		0 180 0

			Physics			1
			PhysGravity		1
			PhysRestitution		.25
			PhysSFriction		.4
			PhysDFriction		.8
			PhysScale		1 1 1
			PhysDensity		2

			#PhysDebris		3

			Drag			0.01
			PhysJointDrag		0.03
			
			PhysJointAnchor		0 0 0
			PhysJointDOFs		RotateX RotateY RotateZ TranslateX TranslateY TranslateZ
			PhysJointAngLimit	30
			PhysJointLinLimit	10
			PhysJointAngSpring	245
			PhysJointAngSpringDamp	10
			PhysJointBreakForce	200
			PhysJointBreakTorque	50
		End

		#JEvent	:IronRodGateJointBreak.fx
	End


End

End