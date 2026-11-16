#############################################################
## Child_ShieldCharge_GroundFX.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	StartOffset
		Type	start
		At	Root
	End
End

#############################################################

#Condition
#	On 	Time
#	Time 	18
#
#	Event
#		EName	GroundCrack
#		Type	start
#		At	Root
#		Bhvr	POWERS\SuperStrength\Splat_FootStomp.bhvr
#		Splat	ImpactCracks.tga
#		Lifespan 40
#	End
#End

#Condition
#	On 	Time
#	Time 	19
#
#	Event
#		Type	start
#		At	StartOffset
#		BhvrOverride
#			Shake          1.5
#			ShakeFallOff   0.075
#			ShakeRadius    128
#		End
#		Lifespan 1
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	45

	Event
		EName	Prime
		Type	start
		At	StartOffset
		BhvrOverride
			TrackRate		10.0
		End
		Part1	POWERS\Shields\ShieldCharge_Trail_Streaks1.part
		Magnet	Root
		LookAt	Root
	End

	Event
		EName	DustTrail
		Type	Local
		At	Prime
		#part1	POWERS\Shields\ShieldCharge_Trail_Rocks1.part
		Part2	POWERS\Shields\ShieldCharge_Trail_Dust1.part
		Part3	POWERS\Shields\ShieldCharge_Trail_Dust2.part
	End

	Event
		EName	Wake
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset		0.0 2.0 0.0
		End
		Part1	POWERS\Shields\ShieldCharge_Trail_Wake1.part
	End
End

#############################################################

#Condition
#	On	PrimeHit
#
#	Event
#		Type	Destroy
#		EName	Prime
#	End
#
#	Event
#		Type	Destroy
#		EName	DustTrail
#	End
#
#	Event
#		Type	Destroy
#		EName	Wake
#	End
#End

#############################################################

End