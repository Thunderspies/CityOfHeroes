#############################################################
## FireSword_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\FireManipulation\FireHandsFlash.part
		Lifespan 5
	End

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\FireManipulation\FireWeapon.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	FX_FireWeapon_Tintable
		Sound FireSword4 50.0 50.0 .88
	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		AltPiv	1
		Part1	CustomizeablePowers\FireManipulation\fireweapon3follow.part
		Part3	CustomizeablePowers\FireManipulation\fireweaponspark.part
		Sound FireSword_loop 50.0 50.0 .33
		POther	SwordSpot2
		PMagnet	SwordSpot2
	End

	Event
		EName 	SwordSpot2
		Type	Local
		At	Prime
		AltPiv	2
		Part1	CustomizeablePowers\FireManipulation\fireweapon1.part
		Part2	CustomizeablePowers\FireManipulation\fireweapon2.part
		Part3	CustomizeablePowers\FireManipulation\fireweapon3.part
		Part4	CustomizeablePowers\FireManipulation\fireweaponWhitness.part
		Part3	CustomizeablePowers\FireManipulation\fireweaponspark.part
		POther	SwordSpot1
		PMagnet	SwordSpot1
	End
End

#############################################################

End