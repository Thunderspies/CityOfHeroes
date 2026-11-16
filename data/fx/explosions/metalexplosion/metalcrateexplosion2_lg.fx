#########################################################
##	onfire
##
FxInfo  

LifeSpan	1200



Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Sound metal_explo2 110 110 .9

		Lifespan	100

	End


	Event
		Ename	Base
		Type	Local 
		At	Origin
	
	End
End


Condition
	On 	Time
	Time 	0
	Repeat	9

	Event
		Type	start 
		At	Base
	
		BhvrOverride
			PositionOffset	0 4 0
			StartJitter	9 4 6
		End

		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

	End

End


###########################################################
############   Metal Box   ################################
##########################################################

Condition
	On 	Time
	Time 	0

############  Software   #########################

############   Top   ################################

	Event
		Ename	lid0
		SoftwareOnly
		Type	start
		At	Base

		Geom	carpart_trailer_panel

		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 8 0		
			StartJitter		2 0 0	
			
			PyrRotate		0 90 90

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25

			SpinJitter		.1 .2 .1

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	lid1
		SoftwareOnly
		Type	start
		At	Base

		Geom	carpart_trailer_panel

		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		5 8 0		
			StartJitter		2 0 0	
			
			PyrRotate		0 90 90

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25

			SpinJitter		.1 .2 .1

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	lid2
		SoftwareOnly
		Type	start
		At	Base

		Geom	carpart_trailer_panel

		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-5 8 0	
			StartJitter		2 0 0

			PyrRotate		0 90 90

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25

			SpinJitter		.1 .2 .1

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

############   Left   ################################

	Event
		Ename	panelL
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		8 4 2

			InitialVelocity		.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 0 0
			
			Spin			0 0 -.15
			SpinJitter		.2 .1 .1

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

	
	Event
		Ename	panelL
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		8 4 -2

			InitialVelocity		.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 0 0
			
			Spin			0 0 -.15
			SpinJitter		.2 .1 .1

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

############   Right   ################################

	Event
		Ename	panelR
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-8 4 2

			InitialVelocity		-.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 0 0
			
			Spin			0 0 .15
			SpinJitter		.2 .1 .1

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

	
	Event
		Ename	panelR
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-8 4 -2

			InitialVelocity		-.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 0 0
			
			Spin			0 0 .15
			SpinJitter		.2 .1 .1

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

############   Front   ################################

	Event
		Ename	panelF
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 4	
			StartJitter		2 0 0

			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25
			
			PyrRotate		0 90 0

			Spin			.15 0 0
			SpinJitter		.1 .1 .2

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

	
	Event
		Ename	pane2F
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		5 4 4	
			StartJitter		2 0 0

			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25
			
			PyrRotate		0 90 0

			Spin			.15 0 0
			SpinJitter		.1 .1 .2

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End
		
	Event
		Ename	pane3F
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-5 4 4	
			StartJitter		2 0 0

			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25
			
			PyrRotate		0 90 0

			Spin			.15 0 0
			SpinJitter		.1 .1 .2

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

############   Back   ################################

	Event
		Ename	panelB
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 -4		
			StartJitter		2 0 0

			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 90 0
			
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	pane2B
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		5 4 -4		
			StartJitter		2 0 0

			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 90 0
			
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

	
	Event
		Ename	pane3B
		SoftwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-5 4 -4		
			StartJitter		2 0 0

			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 90 0
			
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

##############  Hardware   #################################

############   Top   ################################

	Event
		Ename	lid0
		HardwareOnly
		Type	start
		At	Base

		Geom	carpart_trailer_panel

		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 8 0		
			StartJitter		2 0 0	
			
			PyrRotate		0 90 90

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25

			SpinJitter		.1 .2 .1

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

	Event
		Ename	lid1
		HardwareOnly
		Type	start
		At	Base

		Geom	carpart_trailer_panel

		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		5 8 0		
			StartJitter		2 0 0	
			
			PyrRotate		0 90 90

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25

			SpinJitter		.1 .2 .1

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

	Event
		Ename	lid2
		HardwareOnly
		Type	start
		At	Base

		Geom	carpart_trailer_panel

		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-5 8 0	
			StartJitter		2 0 0

			PyrRotate		0 90 90

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25

			SpinJitter		.1 .2 .1

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

############   Left   ################################

	Event
		Ename	panelL
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		8 4 2

			InitialVelocity		.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 0 0
			
			Spin			0 0 -.15
			SpinJitter		.2 .1 .1

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End
	
	Event
		Ename	panelL
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		8 4 -2

			InitialVelocity		.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 0 0
			
			Spin			0 0 -.15
			SpinJitter		.2 .1 .1

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

############   Right   ################################

	Event
		Ename	panelR
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-8 4 2

			InitialVelocity		-.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 0 0
			
			Spin			0 0 .15
			SpinJitter		.2 .1 .1

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

	
	Event
		Ename	panelR
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-8 4 -2

			InitialVelocity		-.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 0 0
			
			Spin			0 0 .15
			SpinJitter		.2 .1 .1

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3

	End

############   Front   ################################

	Event
		Ename	panelF
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 4	
			StartJitter		2 0 0

			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25
			
			PyrRotate		0 90 0

			Spin			.15 0 0
			SpinJitter		.1 .1 .2

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

	
	Event
		Ename	pane2F
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		5 4 4	
			StartJitter		2 0 0

			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25
			
			PyrRotate		0 90 0

			Spin			.15 0 0
			SpinJitter		.1 .1 .2

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End
		
	Event
		Ename	pane3F
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-5 4 4	
			StartJitter		2 0 0

			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25
			
			PyrRotate		0 90 0

			Spin			.15 0 0
			SpinJitter		.1 .1 .2

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

############   Back   ################################

	Event
		Ename	panelB
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 -4		
			StartJitter		2 0 0

			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 90 0
			
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

	Event
		Ename	pane2B
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		5 4 -4		
			StartJitter		2 0 0

			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 90 0
			
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

	
	Event
		Ename	pane3B
		HardwareOnly
		Type	start
		At	Base
	
		Geom	carpart_trailer_panel
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-5 4 -4		
			StartJitter		2 0 0

			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25

			PyrRotate		0 90 0
			
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2

			PhysDebris		1
			PhysDensity		3

		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End
End

############   Contents   ################################

Condition
	On 	Time
	Time 	0

	Repeat	5
	RepeatJitter	2

	Event
		Ename	chunks
		Type	start
		At	Base
		
		Geom    CardBrdBox_genrc_01
		Geom    CardBrdBox_genrc_02
		#Geom    CardBrdBox_genrc_03
		Geom    CardBrdBox_genrc_04
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 0
			StartJitter		8 1.5 8	

			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.75 0.75 0.75

			PyrRotate		0 180 0
			SpinJitter		.2 .2 .2

			StartColor		128 128 128
			Scale			.5 .5 .5

			PhysDebris	1
			PhysDensity	.5

		End

		Lifespan	11
	End

#	Event
#		HardwareOnly
#		Ename	chunks
#		Type	start
#		At	Base
#		
#		Geom	exp_cardboard_panel01
#		Geom	exp_cardboard_panel02
#		
#		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		0 4 0
#			StartJitter		8 1.5 8	
#
#			InitialVelocity		0 0.25 0
#			InitialVelocityJitter	0.5 0.5 0.5
#
#			PyrRotate		0 180 0
#			SpinJitter		.2 .2 .2
#
#			StartColor		128 128 128
#
#			Drag		0.01
#			PhysGravity	.75
#
#			PhysDebris	1
#			PhysDensity	1.5
#
#		End
#
#		Lifespan	11
#	End
End

Condition
	On 	Time
	Time 	0

	Repeat	7
	RepeatJitter	2

	Event
		Ename	chunks
		Type	start
		At	Base
		
		Geom	exp_cardboard_panel01
		Geom	exp_cardboard_panel02
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 0
			StartJitter		8 1.5 8	

			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.5 0.5 0.5

			PyrRotate		0 180 0
			SpinJitter		.2 .2 .2

			StartColor		128 128 128

			Drag		0.01
			PhysGravity	.75

			PhysDebris	1
			PhysDensity	1.5

		End

		Lifespan	11
	End

	Event
		HardwareOnly
		Ename	chunks
		Type	start
		At	Base
		
		Geom	Computer_Monitor_01
		Geom	Computer_PC_01
		Geom	TV_Generic_Med
		Geom	TV_Generic_Sm
		#Geom	FileCabinet_Lg_01
		Geom	FileCabinet_Sm_01
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 0
			StartJitter		8 1.5 8	

			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.5 0.5 0.5

			PyrRotate		0 180 0
			SpinJitter		.2 .2 .2

			StartColor		140 130 120

			PhysDebris	1
			PhysDensity	.75
		End

		Lifespan	11
	End
End


Condition
	On 	Time
	Time 	0

	Repeat	6
	RepeatJitter	2

	Event
		Ename	chunks
		Type	start
		At	Base
		
		Geom	Computer_Monitor_01
		Geom	Computer_PC_01
		Geom	TV_Generic_Med
		Geom	TV_Generic_Sm
		Geom	FileCabinet_Lg_01
		Geom	FileCabinet_Sm_01
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 0
			StartJitter		8 1.5 8	

			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.5 0.5 0.5

			PyrRotate		0 180 0
			SpinJitter		.2 .2 .2

			StartColor		140 130 120

			PhysDebris	1
			PhysDensity	.75
		End

		Lifespan	11
	End

	Event
		HardwareOnly
		Ename	chunks
		Type	start
		At	Base
		
		Geom    CardBrdBox_genrc_01
		Geom    CardBrdBox_genrc_02
		#Geom    CardBrdBox_genrc_03
		Geom    CardBrdBox_genrc_04
		
		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 4 0
			StartJitter		8 1.5 8	

			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.75 0.75 0.75

			PyrRotate		0 180 0
			SpinJitter		.2 .2 .2

			StartColor		128 128 128
			Scale			.5 .5 .5

			PhysDebris	3
			PhysDensity	.5

		End

		Lifespan	11
	End
End


Condition
	On 	Time
	Time 	1

	Event
		Type	start 
		At	root
		
		BhvrOverride
			StartJitter		5 2 2
			PositionOffset		0 4 0
		End

		childFx	Explosions\MetalExplosion\MetalExplosionChild.fx

	End
End

###  Hardware Fire

Condition
	On	Time
	Time 	3
	Repeat	7 #12

	Event
		Ename	parts3
		HardwareOnly
		Type	Start
		At	base
		
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat02
		Geom	Widget_15_Alluminum
		#Geom	Widget_16_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		#Geom	Widget_19_Alluminum
		#Geom	Widget_20_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_22_Alluminum
		#Geom	Widget_23_Alluminum
		Geom	Widget_24_Alluminum
		#Geom	Widget_25_Alluminum
		Geom	Widget_26_Alluminum

		Bhvr	Explosions/VehicleExplosion/PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 0
			StartJitter		3 2 3
			
			StartColor		120 120 130

			PyrRotateJitter		180 180 180

			Scale			.5 .5 .5
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	.75 0.75 .75

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		1
			PhysDensity		2
			
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		#CThresh 0.0001
		
		Lifespan	20
		LifespanJitter	15
	End
End


End			