#########################################################
##	Fireweapon
########################################################
FxInfo
#Lifespan 90


Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		Part1	CustomizeablePowers\FieryMelee\FireHandsFlash.part
		BHVR	behaviors\GenericParticleFade.Bhvr
		Lifespan 5
	End

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\FieryMelee\FireWeapon.bhvr
		BhvrOverride
			TintGeom 	1
		End
		Geom	FX_FireWeapon_Tintable
		Sound FireSword4 50.0 50.0 .88

	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		 AltPiv 1

		Part1	CustomizeablePowers\FieryMelee\fireweapon3follow.part
		Part2	CustomizeablePowers\FieryMelee\fireweapon2.part
		Part3	CustomizeablePowers\FieryMelee\fireweaponspark.part
		Sound FireSword_loop 50.0 50.0 .33
		POther	SwordSpot2
		PMagnet	SwordSpot2
	End


	Event
		EName 	SwordSpot2
		Type	Local
		At	Prime
		 AltPiv 2

		Part	CustomizeablePowers\FieryMelee\fireweapon1.part
		Part	CustomizeablePowers\FieryMelee\fireweapon3.part
		Part	CustomizeablePowers\FieryMelee\fireweaponWhitness.part
		POther	SwordSpot1
		PMagnet	SwordSpot1
	End

	Event
		EName 	SwordSpot3
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\FieryMelee\fireweaponspark.part
		Part2	CustomizeablePowers\FieryMelee\fireweapon2.part
	End
End




End
##################################

