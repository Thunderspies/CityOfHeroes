#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Input
	InpName Hips
End

#############################################################
Condition
	On	Time
	Time	0

	Event
		EName	GunAnchor
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	2.2 0.1 -0.5
		End
	End

	Event
		EName	TargetAnchor
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	20.2 5.0 0.0
		End
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	1


	Event
		EName	TargetStationary
		Type	Start
		At	TargetAnchor
		Lifespan	20
	End

	Event
		Ename	Flash
		Type	StartPositOnly
		At	GunAnchor
		Part	:Muzzle_Flash_Small.part
		Part	:Muzzle_Flash_Shockwave_Small.part
		Lifespan 6

	End

	Event
		Ename 	Prime
		Type	Start
		At	GunAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			TrackRate		3.0
		End
		Part	:FullAuto_Head_Small.part
		Part	:Projectile_Head_Rings_Small.part
		Part	:FullAuto_Trail_Ember.part
		Part	:FullAuto_Trail_Tendril.part
		Magnet	TargetStationary
		LookAt	TargetStationary
		Lifespan 20

	End
End

#############################################################
#
#Condition
#	On	PrimeHit
#
#	Event
#		EName	Prime
#		Type	Destroy
#	End
#End

#############################################################

End