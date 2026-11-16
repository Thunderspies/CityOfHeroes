#########################################################
##	PhysicsRing
##
FxInfo  

#LifeSpan 100




Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound nemesisdrill2_loop 220 220 .8
	End
End

Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound metalexplo1 220 220 .8
	End
End



###################################################
##Smoke from sides
#################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	LeftSideTop
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 0 0
			PositionOffset	9 3.8 -11.5
		End

		Part	:ThinnerSmoke01.part

	End

	Event
		Ename	LeftSideBottem
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 0 0
			PositionOffset	7.8 2.8 -15.5
		End

		Part	:ThinnestSmoke01.part

	End

	Event
		Ename	RightSideTop
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 0 0
			PositionOffset	-5.4 9.8 -14.5
		End

		Part	:Smoke01.part

	End

	Event
		Ename	RightSideBottem
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 0 0
			PositionOffset	-7.4 4.8 -17
		End

		Part	:ThinnestSmoke01.part

	End


End

################################################################
##Right side Metal Bits 
##############################################


Condition
	On 	Time
	Time 	0
#	Repeat	1

	Event
		Type	start 
		At	LeftSideTop
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part


		BhvrOverride
#			PositionOffset	15.5 9 -3
			startJitter	.3 .3 .3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.25 0.45 0.0			
			InitialVelocityJitter	0.1 0.1 0.1
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.5
			physRestitution 	0.5
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.04
		End

		Geom	Metal_Flat01
		
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	151

	End

End

Condition
	On 	Time
	Time 	0
#	Repeat	1

	Event
		Type	start 
		At	LeftSideTop
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part


		BhvrOverride
#			PositionOffset	13.5 10 -3
			startJitter	.3 .3 .3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.25 0.45 0.0			
			InitialVelocityJitter	0.1 0.1 0.1
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.5
			physRestitution 	0.5
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.04
		End

		Geom	Metal_Flat01
		
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	151

	End

End

Condition
	On 	Time
	Time 	0
#	Repeat	1

	Event
		Type	start 
		At	LeftSideBottem
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part


		BhvrOverride
#			PositionOffset	18.5 5 -3.5
			startJitter	.3 .3 .3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.25 0.45 0.0		
			InitialVelocityJitter	0.1 0.1 0.1
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.5
			physRestitution 	0.5
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.04
		End

		Geom	Metal_Flat02
		
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	151

	End

End

#################################################################
###Left side Metal Bits 
###############################################

Condition
	On 	Time
	Time 	0
#	Repeat	1

	Event
		Type	start 
		At	RightSideTop
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part


		BhvrOverride
#			PositionOffset	15 3.5 10.5
			startJitter	.3 .3 .3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		-0.4 0.4 0.0		
			InitialVelocityJitter	0.1 0.1 0.1
			Spin			.05 .051 .13
			SpinJitter		.3 .3 .2
			Drag			0.02
			physics 		2
			physRadius 		0.15
			physGravity 		.5
			physRestitution 	0.5
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.04
		End

		Geom	Metal_Flat01
		Geom	Metal_Flat02
		
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	151

	End

End

Condition
	On 	Time
	Time 	0
#	Repeat	1

	Event
		Type	start 
		At	RightSideTop
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		BhvrOverride
#			PositionOffset	9.5 3.5 11
			Scale		.7 .7 .7
			startJitter	.3 .3 .3
			startcolor		85 75 55

			InitialVelocity		-0.4 0.45 0		
			InitialVelocityJitter	0.2 0.2 0.2
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.02
			physics 		2
			physRadius 		0.15
			physGravity 		.5
			physRestitution 	0.5
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.04
		End

		Geom	Metal_Flat01
		
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	151

	End
End

Condition
	On 	Time
	Time 	0
#	Repeat	1

	Event
		Type	start 
		At	RightSideBottem
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		BhvrOverride
#			PositionOffset	8.5 5.4 10.5
			Scale		.7 .7 .7
			startJitter	.3 .3 .3
			startcolor		85 75 55

			InitialVelocity		-0.4 0.45 0		
			InitialVelocityJitter	0.2 0.2 0.2
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.02
			physics 		2
			physRadius 		0.15
			physGravity 		.5
			physRestitution 	0.5
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.04
		End

		Geom	Metal_Flat02
		
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	151

	End

End


################################################################
##Inner Core Glow
##############################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	-60 0 0
			PositionOffset	0 7.3 -3.2
		End

		Part	:ElectricSparkLightFlares.part

	End
End

################################
##Dirt and Rocks
#######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			PositionOffset	1 1 2
		End

		Part	:EarthDust.part
		Part	:EarthDust2.part
		Part	:EarthDirtClods.part

	End


End

#############################################################
##Sparks
#########################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	-60 0 0
			PositionOffset	0 7.4 -2
		End

		Part	:ElectricSparkLots.part
		Part	:ElectricSpark1.part
	End

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	-60 0 0
			PositionOffset	0 7.4 -4
		End

		Part	:ElectricSparkLots.part
		Part	:ElectricSpark1.part

	End
End

###################################################
##Steam from pipes
#################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 0 0
			PositionOffset	7.4 11.8 -7.9
		End

		Part	:SteamGrey01.part
		Part	:SteamCoreGrey.part

	End

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 0 0
			PositionOffset	-7.4 11.8 -7.9
		End

		Part	:SteamGrey01.part
		Part	:SteamCoreGrey.part

	End

End

###################################################
##Steam from Right Rear Tube
#################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 20 40
			PositionOffset	3.4 1.5 -34.5
		End

		Part	:SmallSteam01.part
		Part	:SmallSteam02.part
		Part	:SmallSteam03.part
		Part	:SteamCoreSmall.part

	End

End

################################

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 95 85
			PositionOffset	2.5 1.8 -34.2
		End

		Part	:SmallSteam01.part
		Part	:SmallSteam02.part
		Part	:SmallSteam03.part
		Part	:SteamCoreSmall.part

	End

End


################################

Condition
	On 	Time
	Time 	27

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	10 70 65
			PositionOffset	4.9 -.8 -35
		End

		Part	:SmallSteam01.part
		Part	:SmallSteam02.part
		Part	:SmallSteam03.part
		Part	:SteamCoreSmall.part

	End

End

###################################################
##Steam from Left Rear Tube
#################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	90 45 50
			PositionOffset	-2.4 2.1 -31.3
		End

		Part	:SmallSteam01.part
		Part	:SmallSteam02.part
		Part	:SmallSteam03.part
		Part	:SteamCoreSmall.part

	End

End

###########################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			Pyrrotate	130 68 60
			PositionOffset	-4.8 -0.4 -33.5
		End

		Part	:SmallSteam01.part
		Part	:SmallSteam02.part
		Part	:SmallSteam03.part
		Part	:SteamCoreSmall.part

	End

End


End
