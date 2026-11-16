#########################################################
##	chill touch hit
FxInfo

##################################
Condition

	Event
		EName 	PhysicRocksHookup
		Type	create
		Inherit	position Rotation scale
		At	Root
	end

End



Condition
#	On	cycle
#	Time	10

	
	Event
		EName 	PhysicRocks
		Type	create
		Inherit	position Rotation scale
		At	PhysicRocksHookup

#		geom	nature_rock_sml_01
#		geom	nature_rock_sml_01
#		geom	nature_rock_sml_02
#		geom	nature_rock_sml_03
#		geom	nature_rock_ledge_flat_sml
#		geom	nature_rock_med
		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05
#		Geom	Cav_rockSM_01
#		Geom	Cav_rockSM_02
#		Geom	Cav_rockSM_03
		
		BhvrOverride
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.14 0.14 0.14
			Scale			.05 .05 .05
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
		LifeSpan	1
	End
		

End				