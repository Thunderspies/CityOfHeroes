#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 240

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
		At	UArmR
		BhvrOverride
			PositionOffset	13.0 -5.0 0.0
		End

	End
End

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
	Time	235

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

End