#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Root
End

##################################
Condition
	On	TriggerBits
	TriggerBits	concrete
	#TriggerBits	grass		#needs grass clipping fx#
	#TriggerBits	gravel		#needs gravel debris fx#
	TriggerBits	Dirt
	TriggerBits	Wood
	#TriggerBits	Carpet
	#TriggerBits	metal

	DoMany		1
		
	Event
		EName 	Prime
		Type	Local 
		At	Root
		Until	Air
		Flags	OneAtaTime

		
		Part4	:SpeedCloudBaseGlow2.part
		Part5	:SpeedCloudBaseGlow3.part
		
	End

End

Condition
	On	TriggerBits
	TriggerBits	concrete
	#TriggerBits	grass		#needs grass clipping fx#
	#TriggerBits	gravel		#needs gravel debris fx#
	TriggerBits	Dirt
	TriggerBits	Wood
	#TriggerBits	Carpet
	#TriggerBits	metal

	DoMany		1
		
	Event
		EName 	PhysicRocks
		Type	Local
		At	Root
		Until	Air
		Flags	OneAtaTime
		
		part	:SprintRocks.part

		BhvrOverride
			InitialVelocity		0.0 0.1 0.0	
			InitialVelocityJitter	.25 0.25 .25
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Gravity			.048
			Physdebris		3
		End
#
##		LifeSpan	220

	End
		
End

###################################	Forces	  ########################

Condition

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		5
			PhysForcePower		60
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	75
		End
	End

	
	Event
		Ename	WakeForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		4
			PhysForcePower		30
			PhysForcePowerJitter	5
			PhysForceSpeedScaleMax	75
		End
	End
	
End				