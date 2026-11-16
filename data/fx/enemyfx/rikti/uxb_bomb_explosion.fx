#########################################################
##	template

FxInfo

LifeSpan	2250

#########################################################
####################### AUDIO ###########################
#########################################################

#Condition
#
#	On 	Time	
#	Time	18
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans4 80 30 .88
#	End
#
#End
#
#Condition
#
#	On 	Time	
#	Time	70
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans3 80 30 .78
#	End
#
#End
#########################################################
################## Physics Geometry #######################
#########################################################
	
Condition
	Repeat	3

	Event
		Type	start
		At	Root
		
		Geom	UXB_Arm_Debris
		Bhvr	Behaviors\UXBPhysics.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
#			PhysDebris		1
			PhysDensity		2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

	End

End

Condition

	Event
		Type	start
		At	Root
		
		Geom	UXB_Core_Debris
		Bhvr	Behaviors\UXBPhysics.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
#			PhysDebris		1
			PhysDensity		2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001


	End
	
	Event
		Type	start
		At	Root
		
		Geom	UXB_Antenna2_Debris
		Bhvr	Behaviors\UXBPhysics.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
#			PhysDebris		1
			PhysDensity		2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001


	End

	Event
		Type	start
		At	Root
		
		Geom	UXB_Antenna_Debris
		Bhvr	Behaviors\UXBPhysics.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
#			PhysDebris		1
			PhysDensity		2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001


	End

	Event
		Type	start
		At	Root
		
		Geom	UXB_lside_Debris
		Bhvr	Behaviors\UXBPhysics.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
#			PhysDebris		1
			PhysDensity		2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001


	End

	Event
		Type	start
		At	Root
		
		Geom	UXB_rside_Debris
		Bhvr	Behaviors\UXBPhysics.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
#			PhysDebris		1
			PhysDensity		2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001


	End
	
	Event
		Type	start
		At	Root
		
		Geom	UXB_middle_Debris
		Bhvr	Behaviors\UXBPhysics.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
#			PhysDebris		1
			PhysDensity		2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001


	End
	
	Event
		Type	start
		At	Root
		
		Geom	UXB_bottem_Debris
		Bhvr	Behaviors\UXBPhysics.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
#			PhysDebris		1
			PhysDensity		2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001


	End
#End

######################################################################################
##Hit Core
#######################################

	Event
		Type	start
		At	root
		
		Part	:HitEnergyFilliments.part
		Part	:UXB_EnergyGasses.part
		Part	:UXB_EnergyGasses2.part
		Part	:UXB_EnergyStreaks_Long.part
		Part	:UXB_EnergyBalls.part
		Part	:UXB_EnergyBalls_Purple.part
		Lifespan	10
		
	End
	
	Event
		Type	start
		At	root
		

		Part	:UXB_EnergyStreak.part
		Part	:LargeEnergyRings.part		

		Lifespan	5
		
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Start
		At	Root

		Part	:LargeEnergyRings.part		

		Lifespan	5
		
	End
End


Condition
	On	Time
	Time	8

	Event
		Type	Start
		At	Root

		Part	:LargeEnergyRings.part		

		Lifespan	5
		
	End
End

###################################################################
################Camera shake on hit
#######################################

Condition

	Event
		Type	start
		At	chest
		
		Bhvr	Behaviors/CameraShake01.bhvr

		Lifespan	1
		
	End

End

End	

#########################################################

	