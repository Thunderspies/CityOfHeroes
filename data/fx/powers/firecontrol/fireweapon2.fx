#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End

	
Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		Part1	:FireHandsFlash.part
		BHVR	Behaviors/GenericParticleFade.Bhvr
		
	End

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Bhvr	behaviors/FireWeapon.bhvr
		Geom	FX_FireWeapon
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

		Part1	:fireweapon1.part
		Part2	:fireweapon2.part
		Part3	:fireweapon3.part
		Part4	:fireweaponWhitness.part
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

