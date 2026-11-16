#########################################################
##	PhysicsRing
##
FxInfo  

#LifeSpan 100
###################################################
##Steam from pipes
#################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 0 0
			PositionOffset	8.4 12 -6.7
		End

		Part	:Steam01.part
		Part	:SteamCore.part

	End

	Event
		Type	Start
		At	Origin
		
		BhvrOverride
			Pyrrotate	30 0 0
			PositionOffset	7.0 11.8 8
		End

		Part	:Steam01.part
		Part	:SteamCore.part

	End

End

################################
##Dirt and Rocks
#######################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Start
#		At	Origin
#		
#		BhvrOverride
#			PositionOffset	3 1 0
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
#	Time 	10
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
#			InitialVelocityJitter	0.8 0.25 0.8
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
#			physics 		1
#			physRadius 		0.3
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.9
#			physDFriction 	 	0.9
#
#		End
#	
#
#		Part	:RockDebrisSandStone.part
#
#
#		Lifespan	75
#		LifespanJitter	55
#		
#		CThresh 	.01
#
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	10
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
#			InitialVelocityJitter	0.8 0.25 0.8
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
#			physics 		1
#			physRadius 		0.3
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.9
#			physDFriction 	 	0.9
#
#		End
#	
#		Part	:RockDebrisSandStone2.part
#
#		Lifespan	75
#		LifespanJitter	55
#		
#		CThresh 	.01
#
#	End
#
#End
#
End
