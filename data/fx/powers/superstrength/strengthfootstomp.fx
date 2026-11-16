#########################################################
## Super Strength - Foot Stomp
########################################################

FxInfo

lifespan 100

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak5
		Type	local
		At	FootR
		Sound footstomp2 100.0 100.0 .85
	End
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UlegR
		Part1	:StrengthFootGlows.part
		PMagnet	LlegR
	End
	
	Event
		EName	Streak2
		Type	local
		At	UlegL
		Part1	:StrengthFootGlows.part
		PMagnet	LlegL
	End

	Event
		EName	Streak3
		Type	local
		At	LlegR
		Part1	:StrengthLowerFootGlows.part
		PMagnet	FootR
	End
	
	Event
		EName	Streak4
		Type	local
		At	LlegL
		Part1	:StrengthLowerFootGlows.part
		PMagnet	FootL
	End


	Event
		EName	Streak5
		Type	local
		At	FootR
		Part1	:StrengthStreak.part
		#Part2	:StrengthPulse.part
		#Part3	:StrengthGlow.part
	End

	Event
		EName	Streak6
		Type	local
		At	FootL
		Part1	:StrengthStreak.part
		#Part2	:StrengthPulse.part
		#Part3	:StrengthGlow.part
	End
End

Condition
	On 	Time
	Time 	26

	Event
		EName	flash
		Type	start
		At	mystic
		#Part1	:AOEflash.part
		#Part2	:AOEflash2.part
		Part3	:FootStompStreaks.part
		Part4	:LightBurst.part
		Part5	:LightBurstFlat.part
	End
End

Condition
	On 	Time
	Time 	26

	Event
		EName	flash
		Type	start
		At	mystic

		Bhvr	Behaviors/CameraShake01.bhvr
	End
End

Condition
	On 	Time
	Time 	23

	Event
		EName	crack
		Type	start
		At	Mystic
		Bhvr	:Splat_FootStomp.bhvr
		Splat	ImpactCracks.tga
		Lifespan	40
	End

	Event
		EName	flash2
		Type	start
		At	Mystic
		Part2	:FootStompRing.part
		Lifespan	25
	End

#	Event
#		EName	flash2
#		Type	start
#		At	crackhookup
#		
#		Bhvr	:FootStompSplat.bhvr
#		Splat	Generic_Ring.tga
#
#		Lifespan	25
#	End
End

## PHYSICS FORCES ######################################################

Condition
	On 	Time
	Time 	29

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Mystic
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		60
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End

	Event
		Ename	BounceForce
		Type	Start
		At	Mystic
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		55
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

## CLEAN UP ######################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	Streak1
		Type	Destroy
	End
	
	Event
		EName	Streak2
		Type	Destroy
	End

	Event
		EName	Streak5
		Type	Destroy
	End

	Event
		EName	Streak6
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	42

	Event
		EName	Streak3
		Type	Destroy
	End

	Event
		EName	Streak4
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	100

	Event
		EName	all
		Type	Destroy
	End
End

########################################################

End