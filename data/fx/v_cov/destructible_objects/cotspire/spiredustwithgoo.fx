#########################################################
##	onfire
##
FxInfo  

#LifeSpan	1150
Flags		InheritAlpha

##########################################################################
###################################################################

Condition

	Event
		Type	start 
		At	Root
		Sound graveldust2 80 80 .88
		Part	:OrganicEnergyFunk.part
		Part	:OrganicEnergy.part
		
	End

	
	Event
		Type	start 
		At	Root
		
		BhvrOverride
			PositionOffset	0 10 0
		End

		Part	:Earthdust.part
		Part	:Earthdust2.part
		LifeSpan	5
		
	End

	Event
		Type	start 
		At	Root
		
#		BhvrOverride
#			PositionOffset	0 10 0
#		End

		Part	:Earthdust.part
		Part	:Earthdust2.part
		LifeSpan	5
		
	End
End

Condition
	Time 	0
	Repeat	11

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Scale			.35 .35 .35
			InitialVelocityJitter	0.25 .7 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 15 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End


Condition
	Time 	0
	repeat	10

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .7 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 5 0
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	repeat	10

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .7 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 5 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	repeat	10

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .7 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 5 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	repeat	10

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .7 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 5 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

End			