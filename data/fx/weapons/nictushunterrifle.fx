#############################################################
## Header
#############################################################

FxInfo

LifeSpan 0

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	wepR
		Sound shotgunout 60 60 .6
	End
End

Condition
	On 	Time
	Time 	4

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_Quantum_Array_Rifle
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

	Event
		EName 	EnergyCore
		Type	Local
		At	Prime
		Geom	NictusHunter
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

	Event
		EName 	Energy
		Type	Local
		At	EnergyCore
		Altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part	:NictusRifleEnergyBLK.part
		Part	:NictusRifleEnergyBLUE.part
		Part	:NictusRifleEnergyPURPLE.part
		Part	:NictusRifleEnergyRED.part
		Sound nictushunterrifle_loop 50 50 .18
	End

	Event
		EName 	StreamEnergy
		Type	Local
		At	EnergyCore
		Altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part	:NictusREDRing.part
		Part	:NictusREDRingExpanding.part
		Part	:NictusRifleEnergyrRed.part
		Part	:NictusRifleEnergyrSTREAMBLK.part
		PMagnet	StreamEnergy2
	End

	Event
		EName 	StreamEnergy2
		Type	Local
		At	EnergyCore
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		BHVR	:HunterSmallRingOffset.bhvr
		Part	:NictusREDRingSmal.part
		Altpiv	2

	End
End

#############################################################

End