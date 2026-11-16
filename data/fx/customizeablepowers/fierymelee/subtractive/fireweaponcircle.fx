#########################################################
##	Fireweapon
########################################################
FxInfo


Condition
	On 	Time
	Time 	0


	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\FieryMelee\FireWeapon.bhvr
		BhvrOverride
			TintGeom 1
		End
		Geom	FX_FireWeapon_Tintable
		Sound FireSword4 60 60 .88

	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		 AltPiv 1

		Part1	CustomizeablePowers\FieryMelee\fireweapon3follow.part
		Part2	CustomizeablePowers\FieryMelee\fireweapon2.part
		Part3	CustomizeablePowers\FieryMelee\fireweaponspark.part
		Sound FireSword_loop 60.0 60.0 .33
		POther	SwordSpot2
		PMagnet	SwordSpot2
	End


	Event
		EName 	SwordSpot2
		Type	Local
		At	Prime
		 AltPiv 2

		Part1	CustomizeablePowers\FieryMelee\fireweapon1.part
		Part2	CustomizeablePowers\FieryMelee\fireweapon2.part
		Part3	CustomizeablePowers\FieryMelee\fireweapon3.part
		Part4	CustomizeablePowers\FieryMelee\fireweaponWhitness.part
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

Condition
	On 	Time
	Time 	5

	Event
		EName	SwordFlash
		Type	Destroy


	End
End


End
##################################

