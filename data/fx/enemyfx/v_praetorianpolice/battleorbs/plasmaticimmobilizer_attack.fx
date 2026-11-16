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
	Time	5

	Event
		EName	MkIIIOffsets
		Type	Local
		At	Chest
		Part	:BattleOrb_Glow.part
		Geom	FX_BattleOrb_MkIII_Offsets
	End

	Event
		Ename	Flash
		Type	StartPositOnly
		At	MkIIIOffsets
		AltPiv	5
		Part	:Muzzle_Flash_Small.part
		Part	:Muzzle_Flash_Shockwave_Small.part
		Sound IDF_BattleOrb_PlasmaImmobilizer_Attack_01 200 200 .8
	End
End

Condition
	On 	Time
	Time 	6



	Event

		EName 	Prime
		Type	start
		At	MkIIIOffsets
		AltPiv	5
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		BhvrOverride
			Scale		0.1 0.1 0.1
			ScaleRate	0.0 0.0 0.0
			EndScale	0.1 0.1 0.1
			Alpha		255
			StartColor	255 255 255
			TrackRate	2.0
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