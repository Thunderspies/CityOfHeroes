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

	Event
		Ename	RockChunkHookupLeft1
		Type	local 
		At	root
		
		BhvrOverride
#			PyrRotate		0 0 60
			PositionOffset	6 18 0
		End

	End

	Event
		Ename	RockChunkHookupLeft2
		Type	local 
		At	root
		
		BhvrOverride
#			PyrRotate		0 0 60
			PositionOffset	-5 20 0
		End

	End

	Event
		Ename	RockChunkHookupLeft3
		Type	local 
		At	root
		
		BhvrOverride
			PositionOffset	10 10 0
		End

	End

	Event
		Ename	RockChunkHookupRight
		Type	local 
		At	root
		
		BhvrOverride
			PositionOffset	-10 4 0
		End

	End

	Event
		Ename	RockChunkHookupLeftLow
		Type	local 
		At	root
		
		BhvrOverride
			PositionOffset	8 4 0
		End

	End

End

########################################################################
#Left Rocks Low
###################################

Condition
#	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeftLow
		
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
		At	RockChunkHookupLeftLow
		
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
		At	RockChunkHookupLeftLow
		
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

########################################################################
#Right Rocks
###################################

Condition
#	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupRight
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		2 2 2
		
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
		At	RockChunkHookupRight
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		2 2 2
		
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
		At	RockChunkHookupRight
		
		Bhvr	Behaviors/fadeout.bhvr
		
		BhvrOverride
		
			StartJitter		2 2 2
		
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

########################################################################
#Left Rocks3
###################################

Condition
#	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft3
		
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
		At	RockChunkHookupLeft3
		
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
		At	RockChunkHookupLeft3
		
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

########################################################################
#Left Rocks1
###################################

Condition
#	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft1
		
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
		At	RockChunkHookupLeft1
		
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
		At	RockChunkHookupLeft1
		
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

#########################################################################
##Left Rocks2
####################################

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
	Repeat	1

	Event
		Type	start 
		At	RockChunkHookupLeft2
		
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
	#Repeat	3

	Event
		Type	start 
		At	RockChunkHookupLeft2
		
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

########################################################################
#####################################################

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