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
			PositionOffset	3 20 0
		End

	End

	Event
		Ename	RockChunkHookupLeft
		Type	local 
		At	root
		
		BhvrOverride
			PyrRotate		0 0 60
			PositionOffset	10 15 0
		End

	End

#	Event
#		Ename	RockChunkHookupLeft2
#		Type	local 
#		At	root
#		
#		BhvrOverride
##			PyrRotate		0 0 60
#			PositionOffset	-10 20 0
#		End
#
#	End
#
#	Event
#		Ename	DustHookupLeft
#		Type	local 
#		At	root
#		
#		BhvrOverride
#			PositionOffset	11 15 0
#		End
#
#	End


End
#################################################
####Dust
#############################
#
#Condition
#	Repeat	2
#
#	Event
#		Type	start 
#		At	DustHookupLeft
#
#		BhvrOverride
#		
#			StartColor		125 125 125
#			InitialVelocity		0.0 0.3 0.0
#			InitialVelocityJitter	0.35 0.25 0.35
#
#			Spin			0.0 0.0 0.0
#			SpinJitter		1 1 1
#
#			Scale			.7 .7 .7
#
#			Drag			0.05
#
#			PyrRotate		0 0 80
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
#		Part	:RockDebrisSandStone.part
#		Part	:RockDebrisSandStone2.part
#		Part	:EarthDustBurst.part
#		Part	:EarthDustBurst2.part
#
#
#
#		LifeSpan	525
#		LifeSpanjitter	55
#	End
#
#End
#
#Condition
#	Repeat	2
#
#	Event
#		Type	start 
#		At	DustHookupLeft
#
#		BhvrOverride
#		
#			StartColor		125 125 125
#			InitialVelocity		0.0 0.3 0.0
#			InitialVelocityJitter	0.35 0.25 0.35
#
#			Spin			0.0 0.0 0.0
#			SpinJitter		1 1 1
#
#			Scale			.7 .7 .7
#
#			Drag			0.05
#
#			PyrRotate		0 0 80
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
#		Part	:RockDebrisSandStone.part
#		Part	:RockDebrisSandStone2.part
#		Part	:EarthDustBurst.part
#		Part	:EarthDustBurst2.part
#
#
#		LifeSpan	525
#		LifeSpanjitter	55
#
#	End
#
#End
#
########################################################################
#Left Rocks
###################################

Condition
#	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.35 0.25 0.35

			Spin			0.0 0.0 0.0
			SpinJitter		1 1 1

			Scale			.7 .7 .7

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
		
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		
		Geom	COT_SmallPortalStone

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
#	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.35 0.25 0.35

			Spin			0.0 0.0 0.0
			SpinJitter		1 1 1

			Scale			.7 .7 .7

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_SmallPortalStone

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	2	#5

	Event
		Type	start 
		At	RockChunkHookupLeft
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		3 2 3
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.35 0.25 0.35

			Spin			0.0 0.0 0.0
			SpinJitter		1 1 1

			Scale			.7 .7 .7

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Geom	COT_SmallPortalStone

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9



		LifeSpan	525
		LifeSpanjitter	55

	End

End

#########################################################################
##Left Rocks2
####################################
#
#Condition
#	Repeat	1
#
#	Event
#		Type	start 
#		At	RockChunkHookupLeft2
#		
#		Bhvr	Behaviors/fadeout.bhvr
#		
#		BhvrOverride
#		
#			StartJitter		3 2 3
#		
#			StartColor		125 125 125
#			InitialVelocity		0.0 0.3 0.0
#			InitialVelocityJitter	0.35 0.25 0.35
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
#		Part	:RockDebrisSandStone.part
#		Part	:RockDebrisSandStone2.part
#		Part	:EarthDustBurst.part
#		Part	:EarthDustBurst2.part
#
#		Geom	COT_SmallPortalStone
#
#		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
#		CThresh .9
#
#
#		LifeSpan	525
#		LifeSpanjitter	55
#
#	End
#
#End
#
#Condition
#	Repeat	1
#
#	Event
#		Type	start 
#		At	RockChunkHookupLeft2
#		
#		Bhvr	Behaviors/fadeout.bhvr
#		
#		BhvrOverride
#		
#			StartJitter		3 2 3
#		
#			StartColor		125 125 125
#			InitialVelocity		0.0 0.3 0.0
#			InitialVelocityJitter	0.35 0.25 0.35
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
#		Part	:RockDebrisSandStone.part
#		Part	:RockDebrisSandStone2.part
#		Part	:EarthDustBurst.part
#		Part	:EarthDustBurst2.part
#
#		
#		Geom	COT_SmallPortalStone
#
#
#		LifeSpan	525
#		LifeSpanjitter	55
#
#	End
#
#End
#
#Condition
#	Repeat	3
#
#	Event
#		Type	start 
#		At	RockChunkHookupLeft2
#		
#		Bhvr	Behaviors/fadeout.bhvr
#		
#		BhvrOverride
#		
#			StartJitter		3 2 3
#		
#			StartColor		125 125 125
#			InitialVelocity		0.0 0.3 0.0
#			InitialVelocityJitter	0.35 0.25 0.35
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
#		Geom	COT_SmallPortalStone
#
#
#
#		LifeSpan	525
#		LifeSpanjitter	55
#
#	End
#
#End
########################################################################

Condition
#	Repeat	2

	Event
		Type	start 
		At	RockChunkHookup
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		1 1 1
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.35 0.25 0.35

			Spin			0.0 0.0 0.0
			SpinJitter		1 1 1

			Scale			.7 .7 .7

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
		
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_SmallPortalStone

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
#	Repeat	2

	Event
		Type	start 
		At	RockChunkHookup
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		1 1 1
		
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.35 0.25 0.35

			Spin			0.0 0.0 0.0
			SpinJitter		1 1 1

			Scale			.7 .7 .7

			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_SmallPortalStone

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	2	#5

	Event
		Type	start 
		At	RockChunkHookup
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		1 1 1
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.35 0.25 0.35

			Spin			0.0 0.0 0.0
			SpinJitter		1 1 1

			Scale			.5 .5 .5

#			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_SmallPortalStone

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

Condition
	Repeat	2	#5

	Event
		Type	start 
		At	RockChunkHookup
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		1 1 1
			StartColor		125 125 125
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.35 0.25 0.35

			Spin			0.0 0.0 0.0
			SpinJitter		1 1 1

			Scale			.9 .9 .9

#			Drag			0.05

#			PyrRotate		0 0 80
		
			FadeOutLength		100

			Gravity			.05
			physics 		1
			physRadius 		0.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9

		End
	
		Part	:RockDebrisSandStone.part
		Part	:RockDebrisSandStone2.part
		Part	:EarthDustBurst.part
		Part	:EarthDustBurst2.part

		Geom	COT_SmallPortalStone

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9


		LifeSpan	525
		LifeSpanjitter	55

	End

End

End			