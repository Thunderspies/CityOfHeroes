#########################################################
## Hamidon - Snipe
#########################################################

FxInfo

LifeSpan	180

#########################################################
## Pre-Fire FX
#########################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	PreFire1
#		Type	Local 
#		At	Waist
#		Part1	:PreFire_Burst.part
#		Part1	:PreFire_Bits.part
#	End
#End
#
#Condition
#	On	Time
#	Time	16
#	Event
#		EName	PreFire1
#		Type 	Local
#		At 	waist
#		Sound	mitoblast1 200 70 1.0
#		Part1	:PreFire_Bits.part
#
#		Lifespan	40
#	End
#End
#
#Condition
#	On 	Time
#	Time 	45
#
#	Event
#		EName	PreFire1
#		Type	Destroy
#	End
#End

#########################################################
## Projectile
#########################################################

#Condition
#	On 	Time
#	Time 	60
#
#	Event
#		EName 	Origin
#		Type	Start 
#		At	Waist
#		Part1	:Fire_Burst.part
#		Part1	:Fire_Core.part
#		Lifespan	5
#	End
#End

Condition
	On	Time
	Time	70

	Event
		EName	Prime
		Type	Start
		At	Waist
		
		Part5	:Beam_Trail_Dark.part
		Part5	:Beam_Trail_Bright.part
		Part2	:Beam_TrailMotes_Dark.part
		Part3	:Beam_TrailMotes_Bright.part
		Part1	:Beam_ProjectileFlare.part

		Geom	HamidonBeam_Projectile
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		BhvrOverride
			TrackRate		8
			TrackMethod		2
		End
		Magnet	Target
		LookAt  Target
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
	End

#########################################################
## Physics
#########################################################

	Event
		Ename	LiftForce
		Type	Local
		At	Prime
		
		BhvrOverride
			pyrRotate		0 0 15
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		
		BhvrOverride
			PositionOffset		0 0 3
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		12
			PhysForcePower		400
			PhysForcePowerJitter	50
		End
	End
End

#########################################################
## Clean-Up
#########################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#########################################################

End			