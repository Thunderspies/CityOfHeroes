#########################################################
##	Stone_Wall
#########################################################

FxInfo

#########################################################
##	Audio
#########################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	Root
#		Sound	Rumble1_loop 100 80 .76
#	End
#	
#End
#
#Condition
#
#	On Cycle
#	Time	46
#	Chance	1
#	
#	Event
#			
#		Type	start
#		At	Root
#		Sound	Quake3 100 80 .64
#		Sound	Quake4 100 80 .76
#		Sound	Quake3 100 80 .68
#		Sound	Rumble4 100 80 .55
#		Sound	Quake5 100 80 .6
#		LifeSpan	46
#		
#	End
#
#End
#
#

#########################################################

LifeSpan	100

Condition
	On Time
	Time	35
	
	Event
		EName 	cameraShake01
		Type	start
		At	Root
		bhvr	Behaviors/EarthQuakeInitial.bhvr
	
		LifeSpan	170
	End

	Event
		EName 	StoneDustRingHit
		Type	local
		At	WepR
		Bhvr	:Offset.bhvr
	End

	Event	
		Type	start
		At	StoneDustRingHit
		Part	:QuakeDustFlatLater2.part
		Part	:QuakeDustLater.part
		Part	:QuakeDust2Later.part
		Part	:QuakeDustFlatLater.part
		Part	:QuakeRadiusRocks.part
		Sound Quake3 100 100 .6
		LifeSpan	150
	End

End

Condition
	On Time
	Time	33

	Event	
		Type	start
		At	T_root

		BHVR	:RingRotate.bhvr
		#Part	:ClubRing.part
		splat	electricring1copy.tga
		LifeSpan	5
	End

	Event	
		Type	start
		At	T_root

		BHVR	:RingRotate2.bhvr
		splat	steam.tga

		LifeSpan	5
	End
End

End


