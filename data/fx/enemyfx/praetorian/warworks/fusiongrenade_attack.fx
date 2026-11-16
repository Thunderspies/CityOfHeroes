#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	24

	Event
		EName	GunAnchor
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	2.2 0.1 -0.5
		End
	End

	Event
		Ename	Flash
		Type	StartPositOnly
		At	GunAnchor
		Part	:Muzzle_Flash_Small.part
		Part	:Muzzle_Flash_Shockwave_Small.part
		Sound WW_ChemGrenade_Attack 200 200 .8
	End
End

Condition
	On 	Time
	Time 	25



	Event

		EName 	Prime
		Type	start
		At	GunAnchor
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			ScaleRate	0.0 0.0 0.0
			EndScale	0.1 0.1 0.1
			Alpha		255
			StartColor	255 255 255
		End
		Part	:Grenade_Core.part
		Part	:Grenade_Globs.part
		Part	:Grenade_Trail_Smoke.part
		Part	:Grenade_Trail_Glow.part
		Part	:Grenade_Rings.part
		Geom	Tintable_SuperStrengthBubble
		Magnet	T_Root
		LookAt	T_Root
	End
End

#############################################################

Condition
	On	PrimeHit
	Event
		Type	Local
		At	T_Root
		ChildFX	:FusionGrenade_Dud.fx
	End

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End