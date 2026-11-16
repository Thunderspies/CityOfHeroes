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
	On 	Time
	Time 	25

	Event
		Type	Local
		At	Root
		Sound WW_WarworksDeath 200 200 .8
	End

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

	End

	Event
		Ename 	Prime
		Type	Start
		At	GunAnchor
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		6.0
		End
		Part	:Projectile_Head_Small.part
		Part	:Projectile_Head_Rings_Small.part
		Part	:Projectile_Trail_Ember_Small2.part
		Part	:Projectile_Trail_Tendril_Small2.part
		Part	:Projectile_Trail_Glow_Small.part
		Part	:Projectile_Trail_Glow_Short.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End