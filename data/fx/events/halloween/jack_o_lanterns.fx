#########################################################
##	Punkins with joint break event of smashing
#########################################################
FxInfo  

Condition
chance	.95

	Event
		EName 	hookup01
		Type	local
		At	root
		BhvrOverride
			PositionOffset	0 0 -1.8
		End
		
	End

End

Condition
chance	.95

	
	Event
		EName 	hookup02
		Type	local
		At	root
		BhvrOverride
			PositionOffset	0 0 1.8
		End
		
	End

End

Condition
chance	.95
	
	Event
		EName 	hookup03
		Type	local
		At	root
		BhvrOverride
			PositionOffset	-1.8 0 0 
		End
		
	End

End

Condition
chance	.95

	Event
		EName 	hookup04
		Type	local
		At	root
		BhvrOverride
			PositionOffset	1.8 0 0 
		End
		
	End

End

########################################################################

Condition
chance	.95
Random	1

	Event
		Ename	Pumpkin_01
		Type	Start
		At	hookup01
		
		BhvrOverride
			PyrRotateJitter	0 30 0	
			StartJitter	.6 0 .6 

			physJointCollidesWorld
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			Gravity			.03
			Drag			0.005

			PhysJointDrag		0.05
			PhysJointAngSpringDamp	120
			PhysJointDOFs		RotateX TranslateX 
			physJointLinSpring	99999999
			physJointLinSpringDamp	999

			PhysJointAngLimit	110
			PhysJointAngSpring	2000

#			PhysDebris		2			
#			PhysJointAnchor		-.5 0 0

#			physJointBreakTorque	9999
			PhysJointBreakForce	 60
			PhysDensity		2

		End

		Geom	Pumpkin_01
		Part	:Candleflamesmall.part
		

		JEvent	:Pumpkin_01_Jointbreak.fx
		JDestroy	1

	End

End


Condition

	Event
		Ename	Rudder
		Type	local
		At	Pumpkin_01
		
		BhvrOverride
			PositionOffset	0 0.4 0 
		End
		Part	:CandleFlickerSmall.part
		
	End
End

Condition
chance	.95

	Event
		Ename	Pumpkin_02
		Type	Start
		At	hookup02
		
		BhvrOverride
			PyrRotateJitter	0 30 0	
			StartJitter	.6 0 .6 
		
			physJointCollidesWorld
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			Gravity			.03
			Drag			0.005

			PhysJointDrag		0.05
			PhysJointAngSpringDamp	120
			PhysJointDOFs		RotateX TranslateX 
			physJointLinSpring	99999999
			physJointLinSpringDamp	999

			PhysJointAngLimit	110
			PhysJointAngSpring	2000

#			PhysDebris		2			
#			PhysJointAnchor		-.5 0 0

#			physJointBreakTorque	9999
			PhysJointBreakForce	 60
			PhysDensity		2

		End

		Geom	Pumpkin_02
		Part	:CandleFlame.Part
		
		JEvent	:Pumpkin_02_Jointbreak.fx
		JDestroy	1
	End

End


Condition
	
	Event
		Ename	Rudder
		Type	local
		At	Pumpkin_02
		
		BhvrOverride
			PositionOffset	0 0.8 0 
		End

		Part	:CandleFlicker.part
		
	End
End

Condition
chance	.95

	Event
		Ename	Pumpkin_03
		Type	Start
		At	hookup03
		
		BhvrOverride
			PyrRotateJitter	0 30 0	
			StartJitter	.6 0 .6 
		
			physJointCollidesWorld
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			Gravity			.03
			Drag			0.005

			PhysJointDrag		0.05
			PhysJointAngSpringDamp	120
			PhysJointDOFs		RotateX TranslateX 
			physJointLinSpring	99999999
			physJointLinSpringDamp	999

			PhysJointAngLimit	110
			PhysJointAngSpring	2000

#			PhysDebris		2			
#			PhysJointAnchor		-.5 0 0

#			physJointBreakTorque	9999
			PhysJointBreakForce	 60
			PhysDensity		2

		End

		Geom	Pumpkin_03
		Part	:Candleflamesmall.part
		
		JEvent	:Pumpkin_03_Jointbreak.fx
		JDestroy	1
	End

End


Condition

	Event
		Ename	Rudder
		Type	local
		At	Pumpkin_03
		
		BhvrOverride	
			PositionOffset	0 0.4 0 
		End

		Part	:CandleFlickerSmall.part
		
	End
End

Condition
chance	.95

	Event
		Ename	Pumpkin_04
		Type	Start
		At	hookup04
		
		BhvrOverride
			PyrRotateJitter	0 30 0	
			StartJitter	.6 0 .6 
		
			physJointCollidesWorld
			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.2
			PhysDFriction		.3
			Gravity			.03
			Drag			0.005

			PhysJointDrag		0.05
			PhysJointAngSpringDamp	120
			PhysJointDOFs		RotateX TranslateX 
			physJointLinSpring	99999999
			physJointLinSpringDamp	999

			PhysJointAngLimit	110
			PhysJointAngSpring	2000

#			PhysDebris		2			
#			PhysJointAnchor		-.5 0 0

#			physJointBreakTorque	9999
			PhysJointBreakForce	 60
			PhysDensity		2

		End

		Geom	Pumpkin_04
		Part	:CandleFlame.Part
		
		JEvent	:Pumpkin_04_Jointbreak.fx
		JDestroy	1

	End

End


Condition

	Event
		Ename	Rudder
		Type	local
		At	Pumpkin_04
		
		BhvrOverride
			PositionOffset	0 0.9 0 
		End

		Part	:CandleFlicker2.part
		
	End

End

End			