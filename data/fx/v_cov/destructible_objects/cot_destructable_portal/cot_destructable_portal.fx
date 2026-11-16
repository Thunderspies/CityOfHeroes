#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150

Condition

	Event
		Ename	RockChunkHookup
		Type	local 
		At	root
		
		BhvrOverride
			PositionOffset	-3 18 0
		End

	End

	Event
		Ename	RockChunkHookupLeft
		Type	local 
		At	root
		
		BhvrOverride
			PyrRotate		0 0 60
			PositionOffset	5 18 0
		End

	End

	Event
		Ename	RockChunkHookupLeft2
		Type	local 
		At	root
		
		BhvrOverride
#			PyrRotate		0 0 60
			PositionOffset	-7 18 0
		End

	End

	Event
		Ename	DustHookupLeft
		Type	local 
		At	root
		
		BhvrOverride
			PositionOffset	10 13 0
		End

	End


End
################################################
###Dust
############################

Condition
	Repeat	1

	Event
		Type	start 
		At	DustHookupLeft

		BhvrOverride
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25

			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
		
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part



		LifeSpan	525
		LifeSpanjitter	55
	End

End

Condition
	Repeat	1

	Event
		Type	start 
		At	DustHookupLeft

		BhvrOverride
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25

			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part


		LifeSpan	525
		LifeSpanjitter	55

	End

End

########################################################################
#Left Rocks
###################################

Condition
	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25

			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
		
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_PortalStone01

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25

			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_PortalStone02

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	5

	Event
		Type	start 
		At	RockChunkHookupLeft
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25


			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Geom	COT_SmallPortalStone



		LifeSpan	525
		LifeSpanjitter	55

	End

End

########################################################################
#Left Rocks2
###################################

Condition
	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft2
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25

			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
		
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_PortalStone01

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft2
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25
			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_PortalStone02

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft2
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25


			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Geom	COT_SmallPortalStone



		LifeSpan	525
		LifeSpanjitter	55

	End

End
#######################################################################

Condition
	Repeat	1

	Event
		Type	start 
		At	RockChunkHookup
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		1 1 1
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25

			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
		
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_PortalStone01

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	1

	Event
		Type	start 
		At	RockChunkHookup
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		1 1 1
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25

			Scale			.6 .8 .6	#1.2 1.2 1.2

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_PortalStone02

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	9

	Event
		Type	start 
		At	RockChunkHookup
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		1 1 1
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.45 0.35 0.45

			#Spin			.5 .5 .5
			SpinJitter		.25 .25 .25

			Scale			.6 .8 .6	#1.2 1.2 1.2

#			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		1
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Geom	COT_SmallPortalStone


		LifeSpan	525
		LifeSpanjitter	55

	End

End

End			