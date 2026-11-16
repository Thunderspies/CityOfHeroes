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
		Part	:Muzzle_Smoke_Chemical.part

		Part	:Muzzle_Flash_Small_Chemical.part
		Part	:Muzzle_Flash_Shockwave_Small_Chemical.part
		Lifespan	7
	End
End

Condition
	On 	Time
	Time 	25



	Event

		EName 	Prime
		Type	Start
		At	GunAnchor
		Bhvr	behaviors\MorterLobbprojectile2.bhvr
		BhvrOverride
			TrackRate	2
		End
		Part	:Grenade_Glow_Chemical.part
		Part	:Grenade_Globs_Chemical.part
		Part	:Grenade_Trail_Smoke_Chemical.part
		Part	:Grenade_Trail_Smoke_Chemical_Full.part
		Geom	grenade
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
		ChildFX	:ChemicalGrenade_Dud.fx
	End

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End