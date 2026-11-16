#############################################################
## DronePlasmaShot_Attack.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	OffsetAnchor
		Type	Local
		Geom	FX_WWMObileDefenseTurret_Offsets
	End

	Event
		Ename	Flash
		Type	Local
		At	OffsetAnchor
		AltPiv	1
		Part	:Muzzle_Glow_Shrinking.part
		Part	:Muzzle_Rings_Shrinking.part

		Part	:Muzzle_Glow_Growing.part
		Lifespan 40
	End
End


Condition
	On	Time
	Time	42

	Event
		Ename	Flash
		Type	Local
		At	OffsetAnchor
		AltPiv	1
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
	End

	Event
		Ename 	Prime
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		BhvrOverride
			TrackRate		8.0
		End
		Part	:Projectile_Smoke_Drone_Small.part
		Part	:Projectile_Head_Drone_Small.part
		Part	:Projectile_Glow_Drone.part
		POther 	Target
		PMagnet	Target
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	Time
	Time	47



	Event
		EName 	BeamGeo
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		Bhvr	behaviors/InfernoBoltBeamSmall.bhvr
		BhvrOverride
			StartColor	0 0 255
			#FadeInLength	5
			FadeOutLength	10

			Scale		0.1 0.1 0.007
			ScaleRate	.05 .05 .05
 			EndScale	.35 .35 0.2
		End
		Part	:Beam_Glow_Drone.part
		geom	Tintable_FireBlast1
		POther	Target
		PMagnet	Target
		Magnet	Target
		LookAt	Target
		LifeSpan		10
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End