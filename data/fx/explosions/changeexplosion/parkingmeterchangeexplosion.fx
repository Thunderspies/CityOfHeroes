#########################################################
##	onfire
##
FxInfo  

LifeSpan	800


Condition
	On 	Time
	Time 	0

############################################################
##Sound####
##################

	Event
		Type	start 
		At	chest
		
		Sound ParkingMeterExplo2 80 80 .9
		Sound ParkingMeterExplo 80 80 .9
		Sound ParkingMeterExplo3 80 80 .9
		Sound ParkingMeterExplo2 80 80 .9
		LifeSpan	5
	End


############################################################
##Fx####
##################

End

Condition

	Event
		Type	start
		At	Root
		Geom	Broken_Parking_Meter
		
		#Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		0 0 0		
			scale			1 1 1

			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0.15 0.15 0.15

			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		510

			StartColor		160 140 120

			
			PhysDebris		1
			PhysDensity		3
		End

		#CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End


Condition
	Time 	0
	Repeat	30

	Event
		Type	start 
		At	Root
		
		Bhvroverride
			PositionOffset		0 4 0
			InitialVelocity		0.0 0.28 0.0		
			InitialVelocityJitter	0.25 0.2 0.25
			pyrRotateJitter		180 180 180
			SpinJitter		.5 1.5 .5
			Drag			0.004
			physics 		1
			physRadius 		.001
			physGravity		.75
			Gravity			.03
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.8
			FadeOutLength		300
			StartColor		255 255 255
			PhysScale		2.5 5 2.5
			PhysDebris		1
			PhysDensity		4

		End
		
		Geom	Geo_coin_01
		Geom	Geo_coin_02
		Geom	Geo_coin_03
		Geom	Geo_coin_01
		Geom	Geo_coin_02
		Geom	Geo_coin_01
		#Part	:QuarterHeads.part

		Lifespan	450
		LifespanJitter	150
	End


	Event
		HardwareOnly
		Type	start 
		At	Root
		
		Bhvroverride
			PositionOffset		0 4 0
			InitialVelocity		0.0 0.28 0.0		
			InitialVelocityJitter	0.25 0.2 0.25
			pyrRotateJitter		180 180 180
			SpinJitter		.5 1.5 .5
			Drag			0.004
			physics 		1
			physRadius 		.001
			physGravity		.75
			Gravity			.03
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.8
			FadeOutLength		300
			StartColor		255 255 255
			PhysScale		2.5 5 2.5

			PhysDebris		1
			PhysDensity		4
		End
		
		Geom	Geo_coin_01
		Geom	Geo_coin_02
		Geom	Geo_coin_03
		Geom	Geo_coin_01
		Geom	Geo_coin_02
		Geom	Geo_coin_01
		#Part	:QuarterHeads.part

		Lifespan	20
		LifespanJitter	15
	End
End

#Condition
#	Time 	0
#	Repeat	12
#
#	Event
#		Type	start 
#		At	Root
#		
#		Bhvroverride
#			PositionOffset		0 4 0
#			InitialVelocity		0.0 0.28 0.0		
#			InitialVelocityJitter	0.15 0.2 0.15
#			Drag			0.004
#			physics 		1
#			physRadius 		.5
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.5
#			physDFriction 	 	0.9
#			FadeOutLength		100
#		End
#
#		Part	:QuarterTails.part
#	End
#
#End
#
#Condition
#	Time 	0
#	Repeat	12
#
#	Event
#		Type	start 
#		At	Root
#		
#		Bhvroverride
#			PositionOffset		0 4 0
#			InitialVelocity		0.0 0.28 0.0		
#			InitialVelocityJitter	0.15 0.2 0.15
#			Drag			0.004
#			physics 		1
#			physRadius 		.25
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.5
#			physDFriction 	 	0.9
#			FadeOutLength		100
#		End
#
#		Part	:DimeHeads.part
#	End
#
#End
#
#Condition
#	Time 	0
#	Repeat	12
#
#	Event
#		Type	start 
#		At	Root
#		
#		Bhvroverride
#			PositionOffset		0 4 0
#			InitialVelocity		0.0 0.28 0.0		
#			InitialVelocityJitter	0.15 0.2 0.15	
#			Drag			0.004
#			physics 		1
#			physRadius 		.25
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.5
#			physDFriction 	 	0.9
#			FadeOutLength		100
#		End
#
#		Part	:DimeTails.part
#	End
#
#
#End
#
#
#		

End			