#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300
#LifeSpanJitter	50

#########################################################
##HardwareOnly
########################################################

Condition

	Event
		Type	local
		At	origin
		
		childFx	Powers/SonicControl/Blasts/SnakeHeadRingCastLongest.fx
		
		LifeSpan	120
	
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	prime
		Type	start
		At	chest
		
		BhvrOverride
			
			TrackRate		.2
			TrackMethod		2
		
		End

		
		LifeSpan	200
		magnet	t_Root
	End

	Event
		EName	Hookup
		Type	start
		At	root

		LifeSpan	120
		
	End

End

Condition
	On 	cycle
	Time 	1

	Event
		EName	FallingRocks
		Type	start
		At	Hookup
		
		childFx	:SmallRocksFalling.fx
		LifeSpan	220
	End

End

#################################################################################
###Both
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Rocks
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset		0 80 0
		End

		ChildFx	:DustFalling.fx
		LifeSpan	90
	End

	Event
		EName	Rocks
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset		0 80 0
		End

		ChildFx	:DustFalling.fx
		LifeSpan	90
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	Rocks
		Type	start
		At	root
		
		Bhvr	Behaviors/SnakeCameraShake04.bhvr
		LifeSpan	300
		
	End

End

###########################################################################################

Condition
	On 	Time
	Time 	35
	Repeat	4
	
	Event
		EName	Rocks
		Type	start
		At	root
		
		Bhvr	:CaveBoulders.bhvr

		BhvrOverride
			
#			Scale			1 1 1
			StartColor		255 180 220

		End

		Geom	_cvsmhlls_rock_xsml_02
		Geom	_cvsmhlls_rock_xsml_01
		
#		CEvent 	V_COV\PhysicsEnabled\RockDebrisSandstoneBreak.fx
#		CThresh .5

	End

End

Condition
	On 	Time
	Time 	37
	Repeat	4
	
	Event
		EName	Rocks
		Type	start
		At	root
		
		Bhvr	:CaveBoulders.bhvr

		BhvrOverride
			
#			Scale			0.5 0.5 0.5
			StartColor		255 180 220

		End

		Geom	_cvsmhlls_rock_xsml_02
		Geom	_cvsmhlls_rock_xsml_01
		
#		CEvent 	V_COV\PhysicsEnabled\RockDebrisSandstoneBreak.fx
#		CThresh .5

	End

End

Condition
	On 	Time
	Time 	36
	
	Event
		EName	Rocks
		Type	start
		At	root
		
		Bhvr	:CaveBoulders.bhvr

		BhvrOverride
			
#			Scale			0.5 0.5 0.5
			StartColor		255 180 220

		End

#		Geom	_cvsmhlls_rock_sml_01
#		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
		
		CEvent 	V_COV\PhysicsEnabled\LargeRockBreak.fx
		CThresh .9
		CDestroy 1


	End

End

Condition
	On 	Time
	Time 	39
	
	Event
		EName	Rocks
		Type	start
		At	root
		
		Bhvr	:CaveBoulders.bhvr

		BhvrOverride
			
#			Scale			0.5 0.5 0.5
			StartColor		255 180 220

		End

#		Geom	_cvsmhlls_rock_sml_01
#		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
		
		CEvent 	V_COV\PhysicsEnabled\LargeRockBreak.fx
		CThresh .9
		CDestroy 1


	End

End

Condition
	On 	Time
	Time 	39
	Repeat	2

	Event
		EName	Rocks
		Type	start
		At	root
		
		Bhvr	:CaveBoulders.bhvr

		BhvrOverride
			
#			Scale			0.5 0.5 0.5
			StartColor		255 180 220

		End

		Geom	_cvsmhlls_rock_sml_01
		Geom	_cvsmhlls_rock_sml_02
		Geom	_cvsmhlls_rock_sml_03
		
		CEvent 	V_COV\PhysicsEnabled\RockDebrisSandstoneBreak.fx
		CThresh .5

	End

End

Condition
	On 	Time
	Time 	35
#	Repeat	3

	Event
		EName	Rocks
		Type	start
		At	root
		
		Bhvr	:CaveBoulders.bhvr

		BhvrOverride

			StartColor		255 180 220

		End

		Geom	_cvsmhlls_rock_xsml_02
		Geom	_cvsmhlls_rock_xsml_01
		
#		CEvent 	V_COV\PhysicsEnabled\RockDebrisSandstoneBreak.fx
#		CThresh .5

	End
	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

End				