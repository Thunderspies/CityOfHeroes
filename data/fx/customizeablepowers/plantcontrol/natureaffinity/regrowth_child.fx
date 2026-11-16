FXinfo
Lifespan 120

##############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME   WeaponOffset
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset 0 0.25 0.5
		End
	End

	Event
		Ename	HeadOffset
		Type	Local
		At	WeaponOffset
		Bhvr	CustomizeablePowers\BeamRifle\Tracer_Fade.bhvr
	End

# LASER SWEEP 1 #######################################################

	Event
		Ename	GroundAnchor
		#Type	Local
		At	Root
		Type CREATE
  		Inherit	Rotation Position
  		Update	Rotation Position
	End

	Event
		Ename	LaserFlash
		Type	Local
		At	HeadOffset
		Lifespan	30
	End

	Event
		Ename	AfterGlowAnchor
		Type	Local
		At	HeadOffset
		Lifespan 1
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	LookAtTarget1
		Type	Local
		At	GroundAnchor
		LookAt	HeadOffset
		BhvrOverride
			InitialVelocity .6 0 1.2
		End
		#Geom Geo_FX_TestSphere
		Lifespan 35
	End

	Event
		Ename	LookAtTarget2
		Type	Local
		At	GroundAnchor
		LookAt	HeadOffset
		BhvrOverride
			InitialVelocity 0.2 0 1.5
		End
		#Geom Geo_FX_TestSphere
		Lifespan 35
	End

	Event
		Ename	LookAtTarget3
		Type	Local
		At	GroundAnchor
		LookAt	HeadOffset
		BhvrOverride
			InitialVelocity -0.2 0 1.5
		End
		#Geom Geo_FX_TestSphere
		Lifespan 35
	End

	Event
		Ename	LookAtTarget4
		Type	Local
		At	GroundAnchor
		LookAt	HeadOffset
		BhvrOverride
			InitialVelocity -.6 0 1.2
		End
		#Geom Geo_FX_TestSphere
		Lifespan 35
	End

#	Event
#		At Root
#		Type Local
#		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
#		BhvrOverride
#			Scale		0.1 0.1 0.1
#			Endscale	3 1 3
#			PositionOffset	0 0 3
#			#StartJitter	3 1 6
#			FadeInLength	120
#			Alpha		255
#			PrimaryTint	80
#			SecondaryTint	20
#		End
#		geom FX_Bush_AzXLgAFol
#		geom FX_Bush_AzSmDDFol
#		Lifespan 75
#	End

	Event
		At Root
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0 0 3
		End
		Part :Regrowth_Grass_Single.part
		Lifespan 1
	End

End

Condition
	On Cycle
	Time 10

	Event
		At LookAtTarget1
		Type StartPositOnly
		Bhvr Behaviors/GenericParticleFade.bhvr
		Part :Regrowth_Grass.part
		Lifespan 5
	End

	Event
		At LookAtTarget2
		Type StartPositOnly
		Bhvr Behaviors/GenericParticleFade.bhvr
		Part :Regrowth_Grass.part
		Lifespan 5
	End

	Event
		At LookAtTarget3
		Type StartPositOnly
		Bhvr Behaviors/GenericParticleFade.bhvr
		Part :Regrowth_Grass.part
		Lifespan 5
	End

	Event
		At LookAtTarget4
		Type StartPositOnly
		Bhvr Behaviors/GenericParticleFade.bhvr
		Part :Regrowth_Grass.part
		Lifespan 5
	End

	Event
		At LookAtTarget1
		Type Start
		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			Endscale	3 2 3
			PositionOffset	0 -1 0
			StartJitter	6 1 6
			FadeInLength	120
			Alpha		255
			PrimaryTint	80
			SecondaryTint	20
		End
		geom FX_Bush_AzMedA
		geom FX_Bush_AzSmBFol
		geom FX_Bush_AzLgAFol
		geom FX_Bush_AzXLgAFol
		geom FX_Bush_AzSmDDFol
		geom FX_Bush_ShrubMedA
		geom FX_Bush_ShrubSmA
		geom FX_Bush_PrivSmBFol
		geom FX_Bush_PrivSmAFol
		geom FX_Bush_WillowLongA
		geom FX_Bush_WillowSmA
		geom FX_Bush_AzLongA
		geom FX_Bush_AzSmA
#		geom FX_Tintable_Bush_AzMedA
#		geom FX_Tintable_Bush_AzSmBFol
#		geom FX_Tintable_Bush_AzLgAFol
#		geom FX_Tintable_Bush_PrivLgAFol
#		geom FX_Tintable_Bush_PrivLgBFol
#		geom FX_Tintable_Bush_AzXLgAFol
#		geom FX_Tintable_Bush_AzSmDDFol
#		geom FX_Tintable_Bush_ShrubMedA
#		geom FX_Tintable_Bush_ShrubSmA
#		geom FX_Tintable_Bush_PrivSmBFol
#		geom FX_Tintable_Bush_PrivSmAFol
		Lifespan 40
	End

	Event
		At LookAtTarget2
		Type Start
		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			Endscale	3 2 3
			PositionOffset	0 -1 0
			StartJitter	6 1 6
			FadeInLength	120
			Alpha		255
			PrimaryTint	80
			SecondaryTint	20
		End
		geom FX_Bush_AzMedA
		geom FX_Bush_AzSmBFol
		geom FX_Bush_AzLgAFol
		geom FX_Bush_AzXLgAFol
		geom FX_Bush_AzSmDDFol
		geom FX_Bush_ShrubMedA
		geom FX_Bush_ShrubSmA
		geom FX_Bush_PrivSmBFol
		geom FX_Bush_PrivSmAFol
		geom FX_Bush_WillowLongA
		geom FX_Bush_WillowSmA
		geom FX_Bush_AzLongA
		geom FX_Bush_AzSmA
	#		geom FX_Tintable_Bush_AzMedA
	#		geom FX_Tintable_Bush_AzSmBFol
	#		geom FX_Tintable_Bush_AzLgAFol
	#		geom FX_Tintable_Bush_PrivLgAFol
	#		geom FX_Tintable_Bush_PrivLgBFol
	#		geom FX_Tintable_Bush_AzXLgAFol
	#		geom FX_Tintable_Bush_AzSmDDFol
	#		geom FX_Tintable_Bush_ShrubMedA
	#		geom FX_Tintable_Bush_ShrubSmA
	#		geom FX_Tintable_Bush_PrivSmBFol
	#		geom FX_Tintable_Bush_PrivSmAFol
		Lifespan 40
	End

	Event
		At LookAtTarget3
		Type Start
		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			Endscale	3 2 3
			PositionOffset	0 -1 0
			StartJitter	6 1 6
			FadeInLength	120
			Alpha		255
			PrimaryTint	80
			SecondaryTint	20
		End
		geom FX_Bush_AzMedA
		geom FX_Bush_AzSmBFol
		geom FX_Bush_AzLgAFol
		geom FX_Bush_AzXLgAFol
		geom FX_Bush_AzSmDDFol
		geom FX_Bush_ShrubMedA
		geom FX_Bush_ShrubSmA
		geom FX_Bush_PrivSmBFol
		geom FX_Bush_PrivSmAFol
		geom FX_Bush_WillowLongA
		geom FX_Bush_WillowSmA
		geom FX_Bush_AzLongA
		geom FX_Bush_AzSmA
	#		geom FX_Tintable_Bush_AzMedA
	#		geom FX_Tintable_Bush_AzSmBFol
	#		geom FX_Tintable_Bush_AzLgAFol
	#		geom FX_Tintable_Bush_PrivLgAFol
	#		geom FX_Tintable_Bush_PrivLgBFol
	#		geom FX_Tintable_Bush_AzXLgAFol
	#		geom FX_Tintable_Bush_AzSmDDFol
	#		geom FX_Tintable_Bush_ShrubMedA
	#		geom FX_Tintable_Bush_ShrubSmA
	#		geom FX_Tintable_Bush_PrivSmBFol
	#		geom FX_Tintable_Bush_PrivSmAFol
		Lifespan 40
	End

	Event
		At LookAtTarget4
		Type Start
		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			Endscale	3 2 3
			PositionOffset	0 -1 0
			StartJitter	6 1 6
			FadeInLength	120
			Alpha		255
			PrimaryTint	80
			SecondaryTint	20
		End
		geom FX_Bush_AzMedA
		geom FX_Bush_AzSmBFol
		geom FX_Bush_AzLgAFol
		geom FX_Bush_AzXLgAFol
		geom FX_Bush_AzSmDDFol
		geom FX_Bush_ShrubMedA
		geom FX_Bush_ShrubSmA
		geom FX_Bush_PrivSmBFol
		geom FX_Bush_PrivSmAFol
		geom FX_Bush_WillowLongA
		geom FX_Bush_WillowSmA
		geom FX_Bush_AzLongA
		geom FX_Bush_AzSmA
	#		geom FX_Tintable_Bush_AzMedA
	#		geom FX_Tintable_Bush_AzSmBFol
	#		geom FX_Tintable_Bush_AzLgAFol
	#		geom FX_Tintable_Bush_PrivLgAFol
	#		geom FX_Tintable_Bush_PrivLgBFol
	#		geom FX_Tintable_Bush_AzXLgAFol
	#		geom FX_Tintable_Bush_AzSmDDFol
	#		geom FX_Tintable_Bush_ShrubMedA
	#		geom FX_Tintable_Bush_ShrubSmA
	#		geom FX_Tintable_Bush_PrivSmBFol
	#		geom FX_Tintable_Bush_PrivSmAFol
		Lifespan 40
	End

	Event
		At LookAtTarget1
		Type Start
		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			Endscale	3 1 3
			PositionOffset	0 0 0
			StartJitter	3 1 6
			FadeInLength	120
			Alpha		255
			PrimaryTint	80
			SecondaryTint	20
		End
		geom FX_Bush_AzXLgAFol
		geom FX_Bush_AzSmDDFol
		Lifespan 40
	End

	Event
		At LookAtTarget2
		Type Start
		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			Endscale	3 1 3
			PositionOffset	0 0 0
			StartJitter	3 0 6
			FadeInLength	120
			Alpha		255
			PrimaryTint	80
			SecondaryTint	20
		End
		geom FX_Bush_AzXLgAFol
		geom FX_Bush_AzSmDDFol
		Lifespan 40
	End

	Event
		At LookAtTarget3
		Type Start
		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			Endscale	3 1 3
			PositionOffset	0 0 0
			StartJitter	1 0 4
			FadeInLength	120
			Alpha		255
			PrimaryTint	80
			SecondaryTint	20
		End
		geom FX_Bush_AzXLgAFol
		geom FX_Bush_AzSmDDFol
		Lifespan 40
	End

	Event
		At LookAtTarget4
		Type Start
		Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			Endscale	3 1 3
			PositionOffset	0 0 0
			StartJitter	1 0 4
			FadeInLength	120
			Alpha		255
			PrimaryTint	80
			SecondaryTint	20
		End
		geom FX_Bush_AzXLgAFol
		geom FX_Bush_AzSmDDFol
		Lifespan 40
	End
End