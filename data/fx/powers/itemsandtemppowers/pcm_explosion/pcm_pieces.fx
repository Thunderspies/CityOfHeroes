#########################################################
##	Punkins with joint break event of smashing
#########################################################
FxInfo  

########################################################################


Condition
	On 	Time
	Time 	0



	Event
		Type	Local
		At	Origin
		Sound PCM_Breakup 70 70 .35
	End



End


Condition

	Event
		Ename	Pumpkin_01
		Type	Start
		At	origin
		
		Bhvr	Behaviors\PumpkinPieces.bhvr
		
		BhvrOverride
			Drag			0.004
			InitialVelocity		0 .3 0
			InitialVelocityJitter	.15 .15 .15
			Gravity			.03
			Scale			1.3 1.3 1.3
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.2
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2
		End

		Geom	PCM_Device_Piece_01
		

	End


	Event
		Ename	Pumpkin_01
		Type	Start
		At	origin
		
		Bhvr	Behaviors\PumpkinPieces.bhvr
		
		BhvrOverride
			Drag			0.004
			InitialVelocity		0 .3 0
			InitialVelocityJitter	.15 .15 .15
			Gravity			.03
			Scale			1.3 1.3 1.3
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.2
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2
		End

		Geom	PCM_Device_Piece_01
		

	End
	
	Event
		Ename	Pumpkin_01
		Type	Start
		At	origin
		
		Bhvr	Behaviors\PumpkinPieces.bhvr
		
		BhvrOverride
			Drag			0.004
			InitialVelocity		0 .3 0
			InitialVelocityJitter	.13 .13 .13
			Gravity			.03
			Scale			1.5 1.5 1.5
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.2
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2
		End

		Geom	PCM_Device_Piece_01
		

	End
	

End

End			