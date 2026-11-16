#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 400

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	PlasmaAnchorR
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	3.4 0.7 0.0
		End
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	StartPositOnly
		At	FootL
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Dust_Circle_Lingering_Light.part
		Part	:Dust_Circle_Lingering_Light2.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	EVENTTEST
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	PlasmaAnchorR
		Part	:Muzzle_Rings_Electricity.part
		Sound WW_ParticleLance_Attack 200 200 .8
		Lifespan 50
	End

	Event
		Type	Local
		At	RArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Muzzle_Glow.part
		Lifespan 40

	End


	Event
		Type	Local
		At	PlasmaAnchorR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Muzzle_Electricity_Core.part
		Part	:Muzzle_Electricity.part
		Lifespan 	65
	End

#############################################################
###################	BEAM	#############################
#############################################################

	Event
		EName 	Flash
		Type	Local
		At	PlasmaAnchorR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Muzzle_Flash.part
		Part	:Muzzle_Flash_Shockwave.part
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	PlasmaAnchorR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Explosion_Flash_Large.part
		Part	:Explosion_Glow.part
		Part	:Explosion_Shockwave_Large.part
		POther	Target
		LookAt	Target
		Lifespan	5
	End
End


Condition
	On	Time
	Time	64

	Event
		EName 	Prime
		Type	Start
		At	PlasmaAnchorR
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		8.0
		End
		Part	:Projectile_Trail_Tendril_Large.part
		Part	:Projectile_Head_Ember_Large.part
		Part	:Projectile_Head_Rings_Large.part
		Part	:Projectile_Trail_Light_Lingering.part
		Part	:Projectile_Trail_Smoke.part

		Magnet	Target
		LookAt	Target
		Lifespan	80
	End

#	Event
#		Type	Local
#		At	PlasmaAnchorR
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Projectile_Trail_Electricity_Blur.part
#		Part	:Projectile_Trail_Electricity_Blur2.part
#		Part	:Projectile_Trail_Electricity.part
#		Part	:Projectile_Trail_Electricity2.part
#		POther	Target
#		LookAt	Target
#		Lifespan	5
#	End
#
#	Event
#		Type	Local
#		At	PlasmaAnchorR
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Projectile_Beam_Core_Short.part
#		Part	:Projectile_Beam_Short.part
#		POther	Target
#		LookAt	Target
#		Lifespan	1
#	End
End

Condition
	On	Time
	Time	180

	Event
		Type	StartPositOnly
		At	FootL
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End

		Part	:Dust_Circle_Lingering_Light.part
		Part	:Dust_Circle_Lingering_Light2.part

		Lifespan 5
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
#
#	Event
#		EName 	Prime2
#		Type	Destroy
#	End
End

#############################################################

End