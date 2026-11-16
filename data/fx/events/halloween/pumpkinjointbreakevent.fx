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
		At	hookup01

		Geom	Pumpkin_01
#		Geom	Pumpkin_02
#		Geom	Pumpkin_03
#		Geom	Pumpkin_04
		
		BhvrOverride

			physJointCollidesWorld
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			Gravity			.03
#			Drag			0.025
#			PhysJointDrag		0.05
			PhysJointAngSpringDamp	220
			PhysJointDOFs		RotateX RotateY RotateZ


#			PhysJointAngLimit	180
#			PhysJointAngSpring	500
#			PhysDebris		2			
#			PhysJointAnchor		-.5 0 0

#			physJointBreakTorque	9999
			PhysJointBreakForce	 40
			PhysDensity		2

		End
		
	End

End

################################################################################3

Condition

	Event
		EName 	hookup01
		Type	local
		At	root
		BhvrOverride
			PositionOffset	0 0 -1.8
		End
		
	End
	
#	Event
#		EName 	hookup02
#		Type	local
#		At	root
#		BhvrOverride
#			PositionOffset	0 0 1.8
#		End
#		
#	End
#
#	Event
#		EName 	hookup03
#		Type	local
#		At	root
#		BhvrOverride
#			PositionOffset	-1.8 0 0 
#		End
#		
#	End
#
#	Event
#		EName 	hookup04
#		Type	local
#		At	root
#		BhvrOverride
#			PositionOffset	1.8 0 0 
#		End
#		
#	End
#
End

########################################################################

End			