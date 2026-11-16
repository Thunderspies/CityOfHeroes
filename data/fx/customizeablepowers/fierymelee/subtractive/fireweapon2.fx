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
		Part1	:FireHandsFlash.part
		BHVR	behaviors\GenericParticleFade.Bhvr
		Lifespan 5
	End

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\FieryMelee\FireWeapon.bhvr
		BhvrOverride
			Alpha		150
			TintGeom 	1
		End
		Geom	FX_Dark_FireWeapon_Tintable
		Sound FireSword4 50.0 50.0 .88

	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		 AltPiv 1

		Part1	:fireweapon3follow.part
		Part2	:fireweapon2.part
		Part3	:fireweaponspark.part
		Sound FireSword_loop 50.0 50.0 .33
		POther	SwordSpot2
		PMagnet	SwordSpot2
	End


	Event
		EName 	SwordSpot2
		Type	Local
		At	Prime
		 AltPiv 2

		Part	:fireweapon1.part
		Part	:fireweapon3.part
		Part	:fireweaponWhitness.part
		POther	SwordSpot1
		PMagnet	SwordSpot1
	End

	Event
		EName 	SwordSpot3
		Type	Local
		At	WepR
		Part1	:fireweaponspark.part
		Part2	:fireweapon2.part
	End
End




End
##################################

