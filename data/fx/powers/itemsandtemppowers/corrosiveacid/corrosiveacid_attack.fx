#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 500

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	wepR
		geom	grenade
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	wepR
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	Powers\ItemsAndTempPowers\GREG_PacificationGrenade\Grenade_Streak.part
		Magnet	T_Root
		LookAt	T_Root
		Sound Corrosive_Attack_01 100 100 .9
	End
End

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	DESTROY
	End

	Event
		Type	Local
		At	T_Root
		Bhvr	behaviors\CameraShake01Subtle.bhvr

		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Explosion_Flash_Large.part
		Part	:Explosion_Flash_Quick.part
		Part	:Explosion_Toxic_Shockwave_Small.part
		Part	:Explosion_Toxic_Fire_Rings_Large.part
		Part	:Explosion_Flash_Large_Flat.part
		Part	:Explosion_Toxic_Shockwave_Flat.part
		Part	:Explosion_Steam_Fill.part
		Part	:Shockwave_Acid.part
		Part	:Toxic_Splash_Initial.part
		Part	:Toxic_Splash_Initial2.part
		Part	:Toxic_Steam.part
		Part	:Glow_Tumors.part
		Part	:Flash_Tumors.part
	#	ChildFX	:RockExplosion.fx
		Lifespan 4
	End

	Event
		EName	DoorAnchor
		Type	Local
		At	T_Root
		GEOM	FX_GEO_LambdaDoorOffsets
		ChildFX	:CorrosiveAcid_Child.fx
		Lifespan	100
	End

	Event
		Type	Local
		At	DoorAnchor
		AltPiv	1
		Part	:Toxic_Splash_Small.part
		Part	:Toxic_Steam_Small.part
		Part	:Toxic_Drip.part
		Lifespan 80
	End

	Event
		Type	Local
		At	DoorAnchor
		AltPiv	2
		Part	:Toxic_Splash_Small.part
		Part	:Toxic_Steam_Small.part
		Part	:Toxic_Drip.part
		Lifespan 70
	End

	Event
		Type	Local
		At	DoorAnchor
		AltPiv	3
		BhvrOverride
			PositionOffset	0.6 0.0 0.0
		End
		Part	:Toxic_Splash_Small.part
		Part	:Toxic_Steam_Small.part
		Part	:Toxic_Drip.part
		Lifespan 90
	End

End

#############################################################

End
