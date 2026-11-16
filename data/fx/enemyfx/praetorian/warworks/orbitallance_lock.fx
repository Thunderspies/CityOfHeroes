#############################################################
## OrbitalLance_Tell.fx
#############################################################

FxInfo
LifeSpan 235

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	75

	Event
		EName	PlasmaAnchorR
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		
		Part	:Muzzle_Rings_Faint.part
		Part	:Muzzle_Tendril_Fast_Blue.part
		Lifespan 	150
		LifespanJitter	10
	End


	Event
		EName	PlasmaAnchorL

		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-2.4 0.7 0.0
		End
		Part	:Muzzle_Rings_Faint.part
		Part	:Muzzle_Tendril_Fast_Blue.part
		Lifespan 	150
		LifespanJitter	10
	End

End

Condition
	On	Time
	Time	90

	Event
		EName	PlasmaAnchorR
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Muzzle_Glow_Constant.part
		Part	:Muzzle_Glow_Core_Constant.part
		Lifespan 	50
		LifespanJitter	10
	End


	Event
		EName	PlasmaAnchorL

		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-2.4 0.7 0.0
		End
		Part	:Muzzle_Glow_Constant.part
		Part	:Muzzle_Glow_Core_Constant.part
		Lifespan 	50
		LifespanJitter	10
	End

End

Condition
	On	Time
	Time	35

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
	On	Time
	Time	135

	Event
		Type	Start
		At	Chest
		Sound WW_OrbitalLance_Attack 200 200 .5
	End
	Event
		Type	Start
		At	PlasmaAnchorL
		ChildFX	:Child_OrbitalLance_Fire_Left.fx
	End

	Event
		Type	Start
		At	PlasmaAnchorR
		ChildFX	:Child_OrbitalLance_Fire.fx
	End
End

Condition
	On	Time
	Time	233

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

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End