#############################################################
## SweepingRay_Attack.fx
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

	Event
		EName	T_Offset
		Type	Local
		At	PlasmaAnchorR
		BhvrOverride
			PositionOffset	5.0 -2.5 0.0
		End
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	StartPositOnly
		At	FootL
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	ENEMYFX\Praetorian\WarworksDust_Circle_Lingering_Light.part
		Part	ENEMYFX\Praetorian\WarworksDust_Circle_Lingering_Light2.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	90

	Event
		EName	EVENTTEST
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	PlasmaAnchorR
		Part	ENEMYFX\Praetorian\WarworksMuzzle_Rings_Electricity.part
		Sound WW_SweepingLaser_Attack 200 200 .7
		Lifespan 50
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	ENEMYFX\Praetorian\WarworksMuzzle_Glow.part
		Lifespan 60

	End


	Event
		Type	Local
		At	PlasmaAnchorR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	ENEMYFX\Praetorian\WarworksMuzzle_Electricity_Core.part
		Lifespan 	75
	End

#############################################################
###################	BEAM	#############################
#############################################################

	Event
		EName 	Flash
		Type	Local
		At	PlasmaAnchorR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	ENEMYFX\Praetorian\WarworksMuzzle_Flash.part
		Part	ENEMYFX\Praetorian\WarworksMuzzle_Flash_Shockwave.part
	End

	Event
		Type	Local
		At	PlasmaAnchorR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	ENEMYFX\Praetorian\WarworksProjectile_Trail_Electricity_Blur.part
		Part	ENEMYFX\Praetorian\WarworksProjectile_Trail_Electricity_Blur2.part
		Part	ENEMYFX\Praetorian\WarworksProjectile_Trail_Electricity.part
		Part	ENEMYFX\Praetorian\WarworksProjectile_Trail_Electricity2.part
		POther	T_Offset
		LookAt	T_Offset
		Lifespan	30
	End

	Event
		Type	Local
		At	PlasmaAnchorR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	ENEMYFX\Praetorian\Warworks:Projectile_Beam_Core.part
		Part	ENEMYFX\Praetorian\WarworksProjectile_Beam.part
		POther	T_Offset
		LookAt	T_Offset
		Lifespan	1
	End

	Event
		Type	Local
		At	T_Offset
		BhvrOverride
			PositionOffset	0.0 -1.5 0.0
		End
		Childfx	ENEMYFX\Praetorian\WarworksChild_SweepingRay_Impact.fx
		Lifespan 45
	End
End

#############################################################

End