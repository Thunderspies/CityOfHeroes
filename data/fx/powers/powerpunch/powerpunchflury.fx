#########################################################
## Energy Melee - Whirling Hands
#########################################################

FxInfo

lifespan	60

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepL
		Sound powerup2 100 100 .8
	End
End

## HANDS #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepL
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3	powers/powerpunch/powerpunch3.part
		Part4	powers/powerpunch/powerpunch2.part
		Part5   powers/powerpunch/powerpunchGlow.part
	End

	Event
		EName 	Prime2
		Type	Local 
		At	WepR
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3	powers/powerpunch/powerpunch3.part
		Part4	powers/powerpunch/powerpunch2.part
		Part5   powers/powerpunch/powerpunchGlow.part
	End

	Event
		EName 	Prime3
		Type	Local 
		At	WepL
		
		Part3   powers/powerpunch/powerpunchRings.part
	End

	Event
		EName 	Prime4
		Type	Local 
		At	WepR
		
		Part3   powers/powerpunch/powerpunchRings.part
	End
End

## GROUND FX #######################################################

#Condition
#	On 	Time
#	Time 	36
#
#	Event
#		EName 	Prime
#		Type	start 
#		At	mystic
#
#		Bhvr	:SplatRing.bhvr
#		Splat	electricring1copy.tga
#
#		LifeSpan	5
#	End
#
#	Event
#		EName 	Prime
#		Type	start 
#		At	mystic
#
#		Bhvr	:SplatRing.bhvr
#		Splat	ThinRing.tga
#
#		LifeSpan	5
#	End
#End

Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime
		Type	start 
		At	mystic

		Part1	powers/powerpunch/HitRingFlat.part
		Part2	powers/powerpunch/HitRingThinFlat.part

		LifeSpan	5
	End
End

## PHYSICS FORCES #######################################################

Condition
	On 	Time
	Time 	36

	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		800
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

## CLEAN-UP #######################################################

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime3
		Type	Destroy
	End

	Event
		EName 	Prime4
		Type	Destroy
	End
End

#########################################################

End		