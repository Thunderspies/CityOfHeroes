#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################
## IDLE

Condition
	On	Cycle
	Time	1

	Event
		Ename 	BigFire
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
		End
		Part	:FireTrail_FlyIdle_Alpha.part
		Part	:FireTrail_FlyIdle_Additive.part
		Part	:FireTrail_FLyIdle_CoreAdd.part
		Lifespan 	6
		LifespanJitter	4
	End
End

############
#Condition
#
#	On 		TRIGGERBITS #Cycle
#	TRIGGERBITS     MALE
#	DoMany		1
#
#	Event
#		ENAME	TrailGround
#		Type	Local
#		At	Root
#		WHILE 	MALE
#		Flags	OneAtATime
#		ChildFX	:Fire_Trail_NOAIR.fx
#	End
#End
#
#Condition
#
#	On 		TRIGGERBITS #Cycle
#	TRIGGERBITS     FEMALE
#	DoMany		1
#
#	Event
#		ENAME	TrailGround
#		Type	Local
#		At	Root
#		WHILE 	FEMALE
#		Flags	OneAtATime
#		ChildFX	:FIRE_TRAIL_NOAIR.fx
#	End
#End
#
#Condition
#
#	On 		TRIGGERBITS #Cycle
#	TRIGGERBITS     HUGE
#	DoMany		1
#
#	Event
#		ENAME	TrailGround
#		Type	Local
#		At	Root
#		WHILE 	HUGE
#		Flags	OneAtATime
#		ChildFX	:FIRE_TRAIL_NOAIR.fx
#	End
#End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FLY
	DoMany		1

	Event
		EName	FireIdle
		Type	Local
		At	Hips
		WHILE	FLY
		Flags	OneAtATime
		ChildFX	:Fire_Flying_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	AIR
	DoMany		1

	Event
		EName	FireIdle
		Type	Local
		At	Hips
		WHILE	AIR
		Flags	OneAtATime
		ChildFX	:Fire_Flying_CHILD.fx
	End
End

#########################################################

End