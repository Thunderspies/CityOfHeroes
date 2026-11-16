#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	Root
	End
	Event
		EName	PlasmaAnchor
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
	End

	Event
		Type	Local
		At	PlasmaAnchor
		Part	:Muzzle_Rings_Plasma.part
		PMagnet	WepR
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	10


	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Muzzle_Glow.part
		Part	:Muzzle_Glow_Core.part
		Sound WW_ChargedShot_Attack 200 200 .8
		Lifespan 30

	End
End


Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	PlasmaAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part	:Muzzle_Tendril_Core.part
		PMagnet	WepR
		Lifespan 15

	End
End
#############################################################

Condition
	On 	Time
	Time 	60

	Event
		EName 	Flash
		Type	Local
		At	PlasmaAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Muzzle_Flash.part
		Part	:Muzzle_Flash_Shockwave.part
	End

	Event
		EName 	Prime
		Type	Start
		At	PlasmaAnchor
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		8.0
		End
		Part	:Projectile_Trail_Ember_Small.part
		Part	:Projectile_Head_Ember.part
		Part	:Projectile_Trail_Tendril.part
		Part	:Projectile_Head_Rings.part
		Part	:Projectile_Trail_Steam.part
		Part	:Projectile_Trail_Light.part

		Magnet	Target
		LookAt	Target
		Lifespan	80
	End
End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End