#########################################################
##	Punkins with joint break event of smashing
#########################################################
FxInfo  

########################################################################

Condition
Random	1

	Event

		Ename	Pumpkin_01
		Type	Start
		At	origin
		
		ChildFx	:SmashedPumpkinPieces.fx
		
	End

	Event

		Ename	Pumpkin_Debris
		Type	Start
		At	origin

		Geom	Pumpkin_Kiss_02
		
		BhvrOverride

			physJointCollidesWorld
			Physics			1
			PhysGravity		.8
			PhysRestitution		.1
			PhysSFriction		.45
			PhysDFriction		.45
			Gravity			.03
			Drag			0.007
#			PhysJointDrag		0.05
			PhysJointAngSpringDamp	220
			PhysJointDOFs		RotateX TranslateX RotateY TranslateY RotateZ TranslateZ 
			PhysScale		2 2 2

			SpinJitter		16 16 16

#			PhysJointAngLimit	180
#			PhysJointAngSpring	500
#			PhysDebris		1		
#			PhysJointAnchor		-.5 0 0

#			physJointBreakTorque	9999
			PhysJointBreakForce	 20
			PhysDensity		2
			LifeSpan		27000

		End
		
	End

End

########################################################################

End			