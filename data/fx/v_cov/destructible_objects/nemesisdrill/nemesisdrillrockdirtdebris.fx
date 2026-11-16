#########################################################
##	PhysicsRing
##
FxInfo  

#LifeSpan 100

Condition
	On 	Cycle
	Time 	10

	Event
		Type	Start
		At	Origin
		
		Bhvr	:CameraShakeSubtle.bhvr
		
		LifeSpan	10

	End

End

#############################################################
##Sparks
#########################################
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	Origin
#		
#		BhvrOverride
#			Pyrrotate	-60 0 0
#			PositionOffset	0 7.5 -2
#		End
#
#		Part	:ElectricSpark1.part
#
#	End
#
#	Event
#		Type	local
#		At	Origin
#		
#		BhvrOverride
#			Pyrrotate	-60 0 0
#			PositionOffset	0 7.5 -4
#		End
#
#		Part	:ElectricSpark1.part
#
#	End
#End

####################################################
###Steam from pipes
##################################
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	Origin
#		
#		BhvrOverride
#			Pyrrotate	30 0 3
#			PositionOffset	7.4 11.8 -7.9
#		End
#
#		Part	:Steam01.part
#		Part	:SteamCore.part
#
#	End
#
#	Event
#		Type	local
#		At	Origin
#		
#		BhvrOverride
#			Pyrrotate	30 0 -3
#			PositionOffset	-7.4 11.8 -7.9
#		End
#
#		Part	:Steam01.part
#		Part	:SteamCore.part
#
#	End
#
#End

#################################
###Dirt and Rocks
########################################################
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	Origin
#		
#		BhvrOverride
#			PositionOffset	1 1 2
#		End
#
#		Part	:EarthDust.part
#		Part	:EarthDust2.part
#		Part	:EarthDirtClods.part
#
#	End
#
#
#End
#
#Condition
#	On 	cycle
#	Time 	5
#	Repeat	10
#
#	Event
#		Type	start
#		At	root
#
#		Bhvr	Behaviors/fadeout.bhvr
#		
#		BhvrOverride
#		
#			StartJitter		3 2 3
#		
#			StartColor		125 125 125
#			InitialVelocity		0.0 0.8 0.0
#			InitialVelocityJitter	0.8 0.5 0.8
#
#			Spin			0.0 0.0 0.0
#			SpinJitter		1 1 1
#
#			Scale			.7 .7 .7
#
#			Drag			0.05
#
##			PyrRotate		0 0 80
#		
#			FadeOutLength		100
#
#			Gravity			.05
##			physics 		1
##			physRadius 		0.3
##			physGravity		1.2
##			physRestitution 	0.5
##			physSFriction 	 	0.9
##			physDFriction 	 	0.9
#
#		End
#	
#
#		Part	:RockDebrisSandStone.part
#
#
#		Lifespan	15
#		LifespanJitter	2
#		
#		CThresh 	.01
#
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	5
#	Repeat	10
#
#	Event
#		Type	start
#		At	root
#
#		Bhvr	Behaviors/fadeout.bhvr
#		
#		BhvrOverride
#		
#			StartJitter		3 2 3
#		
#			StartColor		125 125 125
#			InitialVelocity		0.0 0.8 0.0
#			InitialVelocityJitter	0.8 0.5 0.8
#
#			Spin			0.0 0.0 0.0
#			SpinJitter		1 1 1
#
#			Scale			.7 .7 .7
#
#			Drag			0.05
#
##			PyrRotate		0 0 80
#		
#			FadeOutLength		100
#
#			Gravity			.05
##			physics 		1
##			physRadius 		0.3
##			physGravity		1.2
##			physRestitution 	0.5
##			physSFriction 	 	0.9
##			physDFriction 	 	0.9
#
#		End
#	
#		Part	:RockDebrisSandStone2.part
#
#		Lifespan	15
#		LifespanJitter	2
#		
#		CThresh 	.01
#
#	End
#
#End
#
End
