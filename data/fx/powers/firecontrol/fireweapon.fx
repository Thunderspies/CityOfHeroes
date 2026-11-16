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
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHands.part  
		Sound FireSword6 100.0 100.0 .8
	End
	
	Event
		EName	SwordFlash
		Type	local
		At	WepR
		Part1	:FireHandsFlash.part
	
	End

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Bhvr	behaviors/powers/coldcontrol/IceWeapon.bhvr
		Geom	FX_FireWeapon
		Sound FireSword_loop 100.0 100.0 .7
	
	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		 AltPiv 1
		
		Part1	:fireweapon3follow.part
		Part2	:fireweapon2.part
		Part3	:fireweaponspark.part
		
		BHVR	Behaviors/GenericParticleFade.Bhvr

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
		BHVR	Behaviors/GenericParticleFade.Bhvr
	End

Event
		EName 	SwordSpot3
		Type	Local
		At	WepR
		Part1	:fireweaponspark.part
		Part2	:fireweapon2.part

		BHVR	Behaviors/GenericParticleFade.Bhvr
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	SwordFlash
		Type	Destroy
		
	
	End
End

Condition
	On 	Time
	Time 	23

	Event
		EName	InitialSwordFire
		Type	Destroy
		
	
	End

	End
End
##################################

