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
	Time 	11

	Event
		Type	Local
		At	Root
		sound Plasma_Blast 200 200 .8
	End

	Event
		Ename	Flash
		Type	Local
		At	C_Head
		Part	ENEMYFX\Clockwork\antimatter\Muzzle_Flash_Small.part
	End

	Event
		Ename 	Prime
		Type	Start
		At	C_Head
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0
		End
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Ember_Small.part
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Tendril_Small.part
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Glow_Small.part
		Magnet	Target
		LookAt	Target
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	16

	Event
		Ename	Flash
		Type	Local
		At	C_Neck
		Part	ENEMYFX\Clockwork\antimatter\Muzzle_Flash_Small.part
	End

	Event
		Ename 	Prime1
		Type	Start
		At	C_Neck
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0

		End
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Ember_Small.part
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Tendril_Small.part
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Glow_Small.part
		Magnet	Target
		LookAt	Target
		Lifespan 40
	End
End


Condition
	On 	Time
	Time 	22

	Event
		Ename	Flash
		Type	Local
		At	C_Head
		Part	ENEMYFX\Clockwork\antimatter\Muzzle_Flash_Small.part
	End

	Event
		Ename 	Prime2
		Type	Start
		At	C_Head
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0
		End
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Ember_Small.part
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Tendril_Small.part
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Glow_Small.part
		Magnet	Target
		LookAt	Target
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	27

	Event
		Ename	Flash
		Type	Local
		At	C_Neck
		Part	ENEMYFX\Clockwork\antimatter\Muzzle_Flash_Small.part
	End

	Event
		Ename 	Prime3
		Type	Start
		At	C_Neck
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0
		End
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Ember_Small.part
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Tendril_Small.part
		Part	ENEMYFX\Clockwork\antimatter\Projectile_Trail_Glow_Small.part
		Magnet	Target
		LookAt	Target
		Lifespan 40
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

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
	End
End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit

	Event
		EName 	ALL
		Type	Destroy
	End
End

#############################################################

End