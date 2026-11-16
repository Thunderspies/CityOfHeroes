#############################################################
## FireWeapon3.fx
#############################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		Part1	:FireHandsFlash.part
		BHVR	behaviors\GenericParticleFade.Bhvr

	End

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		Sound glow3_loop 80 80 .33
	End

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\FieryMelee\FireWeapon.bhvr
		BhvrOverride
			Alpha		200
			TintGeom 	1
		End
		Geom	FX_Dark_FireWeaponGreater_Tintable
	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		 AltPiv 1
		Sound FireSword_loop 60.0 60.0 .33
		Part1	:fireweapon3followa.part
		Part3	:fireweaponspark.part


		POther	SwordSpot2
		PMagnet	SwordSpot2
	End


	Event
		EName 	SwordSpot2
		Type	Local
		At	Prime
		 AltPiv 2

		Part1	:fireweapon1a.part
		Part2	:fireweapon2a.part
		Part3	:fireweapon3a.part
		Part5	:fireweaponBlueGlow.part
		POther	SwordSpot1
		PMagnet	SwordSpot1
	End

	Event
		EName 	SwordSpot3
		Type	Local
		At	WepR
		Part1	:fireweaponspark.part
		Part2	:fireweapon2a.part
	End

End

Condition
	On 	Time
	Time	2

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		Sound Firestreaks3 80.0 80.0 .6
		Lifespan 5
	End

End

########################################################

End


